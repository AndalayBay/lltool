#ifndef _PLLMAPLIST_H_
#define _PLLMAPLIST_H_

#include <vector>

#include "../include/llmap.h"
#include "../include/llpolygonlist.h"
#include "../include/lllinelist.h"
#include "../include/lltrianglelist.h"

class       llMapList;
llMapList* _llMapList();
llMapList& _fllMapList();

class llMapList {

protected:

	std::vector<llMap*>          map_list;
	std::vector<const char*>     map_name;
	std::vector<llPointList*>    point_list;
	std::vector<llPolygonList*>  polygon_list;
	std::vector<llLineList*>     line_list;
	std::vector<llTriangleList*> triangle_list;


public:

	//constructor
	llMapList();

	int AddMap(const char *_name, llMap *_map, llPointList *_points, llTriangleList *_triangles, llPolygonList *_polygons, llLineList *_lines);
	int AddMap(const char *_name, llMap *_map, const char *_oldmap);

	int DeleteMap(char *_name);
	void ExchangeMap(char *_name, llMap *_map);

	int DeleteTriangles(char *_name);

	llMap *GetMap(const char* _name);

	int GetMapNum(const char* _name) {
		for (int i=0; i<GetSize(); i++) {
			if (_stricmp(_name, map_name[i]) == 0)
				return i;
		}
		return -1;
	}

	int GetSize(void) {
		return map_list.size();
	}

	llPointList *GetPointList(char *_map) {
		int map_p = GetMapNum(_map);
		if (map_p<0) return NULL;
		return point_list[map_p];
	}
	llLineList *GetLineList(char *_map) {
		int map_p = GetMapNum(_map);
		if (map_p<0) return NULL;
		return line_list[map_p];
	}
	llTriangleList *GetTriangleList(char *_map) {
		int map_p = GetMapNum(_map);
		if (map_p<0) return NULL;
		return triangle_list[map_p];
	}
	llPolygonList *GetPolygonList(char *_map) {
		int map_p = GetMapNum(_map);
		if (map_p<0) return NULL;
		return polygon_list[map_p];
	}

	float GetX1() {
		llMap * map = GetMap("_heightmap");
		if (!map) return 0.f;
		return map->GetX1();
	};
	float GetY1() {
		llMap * map = GetMap("_heightmap");
		if (!map) return 0.f;
		return map->GetY1();
	};
	float GetX2() {
		llMap * map = GetMap("_heightmap");
		if (!map) return 0.f;
		return map->GetX2();
	};
	float GetY2() {
		llMap * map = GetMap("_heightmap");
		if (!map) return 0.f;
		return map->GetY2();
	};
	float GetWidthXPerRaw() {
		llMap * map = GetMap("_heightmap");
		if (!map) return 0.f;
		return map->GetWidthXPerRaw();
	};
	float GetWidthYPerRaw() {
		llMap * map = GetMap("_heightmap");
		if (!map) return 0.f;
		return map->GetWidthYPerRaw();
	};

	int GetNumHeights(char *_mapname, float _x, float _y, double *_angles, float *_z, int _size);

};

#endif
