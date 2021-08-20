#include <iostream>
#include <sstream>
#include <geojson.hh>
#include <errno.h>
#include <unistd.h>
#include <cmath>

#define CAPITALS_GEOJSON "./capitals.geojson"
#define COUNTRIES_GEOJSON "./countries.geojson"

#define CODE_FAILED 1
#define CODE_SUCCESS 0

std::string type_border("border");
std::string type_cable("cable");

double distance(coord_t coord1, coord_t coord2) {
	double x_coord = pow(coord1.x - coord2.x, 2);
	double y_coord = pow(coord1.y - coord2.y, 2);
	return sqrt(x_coord + y_coord);
}

double calc_polygon_sz(polygon_t polygon) {
	size_t coords_len = polygon.m_coord.size();
	size_t last_coord = coords_len - 1;
	double border_len = 0;

	for (size_t idx_coord = 1; idx_coord < coords_len; idx_coord++) {
		border_len += distance(polygon.m_coord.at(idx_coord-1), polygon.m_coord.at(idx_coord));
	}

	border_len += distance(polygon.m_coord.at(last_coord),
							polygon.m_coord.at(0));

	return border_len;
}

double calc_shortest_distance(polygon_t pct, coord_t ccap) {
	size_t coords_len = pct.m_coord.size();
	double tmp;
	double min_dist = 0;

	for (size_t idx_coord = 0; idx_coord < coords_len; idx_coord++) {
		tmp = distance(ccap, pct.m_coord.at(idx_coord));
		if ((min_dist == 0) || (tmp < min_dist)) {
			min_dist = tmp;
		}
	}

	return min_dist;
}

double calc_len(feature_t feature) {
	geometry_t geometry;
	polygon_t polygon;
	double border_len = 0;
	size_t size_geometry ;
	size_t size_pol;

	size_geometry = feature.m_geometry.size();
	for (size_t idx_geo = 0; idx_geo < size_geometry; idx_geo++) {
		geometry = feature.m_geometry.at(idx_geo);
		size_pol = geometry.m_polygons.size();

		for (size_t idx_pol = 0; idx_pol < size_pol; idx_pol++) {
			polygon = geometry.m_polygons[idx_pol];
			border_len += calc_polygon_sz(polygon);
		}
	}

	return border_len;
}

void output_json_result(std::unordered_map<std::string, double> len, std::unordered_map<std::string,feature_t> features, std::string calc_type) {
	feature_t feature;
	std::unordered_map<std::string, feature_t>:: iterator itr;
	std::ostringstream json_res_stream;
	std::string json_res;

	json_res_stream << "[";
	for (itr = features.begin(); itr != features.end(); itr++) {
		feature = itr->second;
		json_res_stream << "{\"name\":\"" << feature.m_admin << "\"," << "\"iso_a3\":\"" << feature.m_iso_a3 << "\"," << "\"" << calc_type << "\":" << len[itr->first] << "},";
	}

	json_res = json_res_stream.str();
	json_res.pop_back();
	json_res += "]";

	std::cout << json_res << std::endl;
}

void output_border_len(double len, feature_t feature, std::string iso_a3) {
	std::unordered_map<std::string, double> border_len;
	std::unordered_map<std::string,feature_t> features;

	border_len[iso_a3] = len;
	features[iso_a3] = feature;

	output_json_result(border_len, features, "border");
}

void calc_border_len(geojson_t geojson, std::string iso_a3) {
	feature_t feature;
	double len;
	
	feature = geojson.m_feature.at(iso_a3);

	len = calc_len(feature);

	output_border_len(len, feature, iso_a3);
}

void calc_all_borders(geojson_t geojson) {
	std::unordered_map<std::string, double> lens;
	std::unordered_map<std::string, feature_t>:: iterator itr;		

	for (itr = geojson.m_feature.begin(); itr != geojson.m_feature.end(); itr++) {
		lens[itr->first] = calc_len(itr->second);
	}

	output_json_result(lens, geojson.m_feature, "border");
}

void output_cable_len(double len, feature_t feature, std::string iso_a3) {
	std::unordered_map<std::string, double> cable_len;
	std::unordered_map<std::string,feature_t> features;

	cable_len[iso_a3] = len;
	features[iso_a3] = feature;

	output_json_result(cable_len, features, "cable");
}

void calc_cable_len(geojson_t geojson_capitals, geojson_t geojson_countries, std::string iso_a3) {
	feature_t fcap, fct;
	geometry_t gcap, gct;
	polygon_t pcap, pct;
	//coord_t ccap;
	size_t gsz, pol_sz;
	double tmp;
	double cable_len = 0;
	
	fcap = geojson_capitals.m_feature.at(iso_a3);

	// only 1 geometry, polygon and coordinates for capitals
	gcap = fcap.m_geometry.at(0);
	pcap = gcap.m_polygons.at(0);
	//ccap = pcap.m_coord.at(0);

	fct = geojson_countries.m_feature.at(iso_a3);
	gsz = fct.m_geometry.size();
	for (size_t idx_geo = 0; idx_geo < gsz; idx_geo++) {
		gct = fct.m_geometry.at(idx_geo);
		pol_sz = gct.m_polygons.size();

		for (size_t idx_pol = 0; idx_pol < pol_sz; idx_pol++) {
			pct = gct.m_polygons.at(idx_pol);
			tmp = calc_shortest_distance(pct, pcap.m_coord.at(0));
			if ((cable_len == 0) || (tmp < cable_len))
				cable_len = tmp;
		}

	}

	output_cable_len(cable_len, fcap, iso_a3);
}

std::string getString(char x) {
	std::string s(1, x);
	return s;
}

#define _printable(ch) (isprint((unsigned char) ch) ? ch : '#')
#define	printable(ch)	(getString(_printable(ch)))


int main(int argc, char* argv[]) {
	int opt, xfnd;
	std::string iso, calc_type;
	geojson_t geojson_countries;
	geojson_t geojson_capitals;

	xfnd = 0;
	
	while ((opt = getopt(argc, argv, ":c:t:")) != -1) {
		switch (opt) {
		case 'c': iso = std::string(optarg); break;
		case 't': calc_type = std::string(optarg); break;
		case ':': std::cerr << "Missing argument: " << printable(optopt) << std::endl; exit(CODE_FAILED); 
		case '?': std::cerr << "Unrecognized option: " << printable(optopt) << std::endl; exit(CODE_FAILED);
		default: break;
		}	
	}

	if (geojson_countries.convert(COUNTRIES_GEOJSON) < 0)
		exit(CODE_FAILED);

	if (geojson_capitals.convert(CAPITALS_GEOJSON) < 0)
		exit(CODE_FAILED);


	if (iso.empty())
		calc_all_borders(geojson_countries);
	else if (!calc_type.empty()) {
		if (type_border.compare(calc_type) == 0)
			calc_border_len(geojson_countries, iso);
		else if (type_cable.compare(calc_type) == 0)
			calc_cable_len(geojson_capitals, geojson_countries, iso);
		else
			std::cerr << "Invalid calculation type. Valid type: boder, cable" << std::endl;
	}
	else {
		std::cerr << "Calculation type not provided" << std::endl;
	}	
}
