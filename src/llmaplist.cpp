//a list of algorithms

#include "../include/llmaplist.h"
#include <string.h>
#include <stdio.h>


llMapList& _fllMapList() {
    static llMapList* ans = new llMapList();
    return *ans;
}

llMapList * _llMapList() {
    return &_fllMapList();
}

llMapList::llMapList() {
	map_list.resize(0);
	map_name.resize(0);
}

int llMapList::AddMap(const char *_name, llMap *_map, llPointList *_points, llTriangleList *_triangles, llPolygonList *_polygons, llLineList *_lines) {
	int size = GetSize();
	for (int i=0; i<size; i++) {
		if (_stricmp(_name, map_name[i]) == 0) {
			_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::AddMap: map %s already existing", _name);
			return 0;
		}
	}
	map_list.resize(size + 1);
	map_list[size]      = _map;
	map_name.resize(size + 1);
	map_name[size]      = _name;
	point_list.resize(size + 1);
	point_list[size]    = _points;
	line_list.resize(size + 1);
	line_list[size]     = _lines;
	triangle_list.resize(size + 1);
	triangle_list[size] = _triangles;
	polygon_list.resize(size + 1);
	polygon_list[size]  = _polygons;
	return 1;
}

int llMapList::AddMap(const char *_name, llMap *_map, const char *_oldmap) {
	int size = GetSize();
	for (int i=0; i<size; i++) {
		if (_stricmp(_name, map_name[i]) == 0) {
			_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::AddMap: map %s already existing", _name);
			return 0;
		}
	}
	int num = GetMapNum(_oldmap);
	if (num<0) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::AddMap: source map %s not found", _name);
		return 0;
	}
	map_list.resize(size + 1);
	map_list[size]      = _map;
	map_name.resize(size + 1);
	map_name[size]      = _name;
	point_list.resize(size + 1);
	point_list[size]    = point_list[num];
	line_list.resize(size + 1);
	line_list[size]     = line_list[num];
	triangle_list.resize(size + 1);
	triangle_list[size] = triangle_list[num];
	polygon_list.resize(size + 1);
	polygon_list[size]  = polygon_list[num];

	return 1;
}

void llMapList::ExchangeMap(char *_name, llMap *_map) {
	int size = GetSize();
	for (int i=0; i<size; i++) {
		if (_stricmp(_name, map_name[i]) == 0) {
			map_list[i] = _map;
			if (polygon_list[i]) polygon_list[i]->SetMap(_map);
			return;
		}
	}
	_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::ExchangeMap: map %s not found", _name);
}


llMap *llMapList::GetMap(const char* _name) {
	//cout << "GetMap " << _name  << endl;
	for (int i=0; i<GetSize(); i++) {
		if (_stricmp(_name, map_name[i]) == 0)
			return map_list[i];
	}
	return NULL;
}

int llMapList::DeleteMap(char *_name) {
	//deletes the map extry, and deletes also the object(s)
	//if not shared with other entries.

	int num = GetMapNum(_name);
	if (num<0) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::DeleteMap: map %s not found", _name);
		return 0;
	}

	llMap          *map       = map_list[num];
	llPointList    *points    = point_list[num];
	llTriangleList *triangles = triangle_list[num];
	llLineList     *lines     = line_list[num];
	llPolygonList  *poly      = polygon_list[num];

	for (int i=0; i<GetSize(); i++) {
		if (i != num) {
			if (map == map_list[i])            map       = NULL;
			if (points == point_list[i])       points    = NULL;
			if (lines  == line_list[i])        lines     = NULL;
			if (triangles == triangle_list[i]) triangles = NULL;
			if (poly == polygon_list[i])       poly      = NULL;
		}
	}

	if (!map) delete map;
	if (!points) delete points;
	if (!lines) delete lines;
	if (!triangles) delete triangles;
	if (!poly) delete poly;

	for (int i=num; i<GetSize()-1; i++) {
		map_name[i]      = map_name[i+1];
		map_list[i]      = map_list[i+1];
		point_list[i]    = point_list[i+1];
		line_list[i]     = line_list[i+1];
		triangle_list[i] = triangle_list[i+1];
		polygon_list[i]  = polygon_list[i+1];
	}

	int size = GetSize();

	map_list.resize(size - 1);
	map_name.resize(size - 1);
	point_list.resize(size - 1);
	line_list.resize(size - 1);
	triangle_list.resize(size - 1);
	polygon_list.resize(size - 1);

	return 1;

}

int llMapList::GetNumHeights(char *_mapname, float _x, float _y, float *_angles, float *_z, int _size) {
	//returns the number of possible height of the map _mapname
	//at position _x, _y

	llLineList *lines = GetLineList(_mapname);
	if (!lines) return 1; //no lines, no problem...

	llPointList *points = GetPointList(_mapname);
	if (!points) return 1;

	llMap *map = GetMap(_mapname);

	int angles_counter = 0;

	float pixelsize = (map->GetWidthXPerRaw() + map->GetWidthXPerRaw()) / 2.f;

	for (int i=0; i<lines->GetN(); i++) {
		float t, px, py, pz;

		points->GetClosestPointOnLine(lines->GetLine(i)->GetX1(), lines->GetLine(i)->GetY1(), 0,
			lines->GetLine(i)->GetX2() - lines->GetLine(i)->GetX1(), lines->GetLine(i)->GetY2() - lines->GetLine(i)->GetY1(), 0,
			_x, _y, 0, &t, &px, &py, &pz);

		float distance = sqrt((px - _x)*(px - _x) + (py - _y)*(py - _y));

		if (distance <= pixelsize && t >= 0 && t <= 1) {
			float angle = atan((lines->GetLine(i)->GetY2() - lines->GetLine(i)->GetY1()) / 
				(lines->GetLine(i)->GetX2() - lines->GetLine(i)->GetX1()));
			if (angles_counter < _size-1) {
				_angles[angles_counter++] = angle;

				if (sqrt((px - lines->GetLine(i)->GetX1())*(px - lines->GetLine(i)->GetX1()) + (py - lines->GetLine(i)->GetY1())*(py - lines->GetLine(i)->GetY1())) > pixelsize &&
					sqrt((px - lines->GetLine(i)->GetX2())*(px - lines->GetLine(i)->GetX2()) + (py - lines->GetLine(i)->GetY2())*(py - lines->GetLine(i)->GetY2())) > pixelsize) {
						//in the middle of the line
						_angles[angles_counter++] = angle + M_PI;
				}

			} else {
				_llLogger()->WriteNextLine(-LOG_ERROR, "too many discontinuities in map [%s] at (%f, %f)", _mapname, _x, _y);
			}
		}
	}

	for (int i=0; i<angles_counter; i++) {
		if (_angles[i] < 0)      _angles[i] += 2*M_PI;
		if (_angles[i] > 2*M_PI) _angles[i] -= 2*M_PI;
	}

	//bubble sort
	for (int i=0; i<angles_counter; i++) {
		for (int j=0; j<i-1; j++) {
			if (_angles[j] > _angles[j+1]) {
				double x     = _angles[j];
				_angles[j]   = _angles[j+1];
				_angles[j+1] = x;
			}
		}
	}

	//calculate section angles
	float *secangles = new float[_size];

	if (_angles[0] == 0) {
		secangles[0] = _angles[1] / 2.;
	} else { //first section angle must take the last one into account
		secangles[0] = (_angles[0] + (_angles[angles_counter-1] - 2*M_PI)) / 2.;
		if (secangles[0] < 0) secangles[0] += 2*M_PI;
	}

	for (int i=1; i<angles_counter; i++) {
		secangles[i] = (_angles[i] + _angles[i-1]) / 2.;
	}

	//find pixel
	unsigned int posx = map->GetRawX(_x);
	unsigned int posy = map->GetRawX(_y);

	for (int i=0; i<angles_counter; i++) {
		unsigned int loc_posx = posx;
		unsigned int loc_posy = posy;
		if (secangles[i] < M_PI/8.)          {loc_posx++;}
		else if (secangles[i] < M_PI*3./8.)  {loc_posx++;loc_posy++;}
		else if (secangles[i] < M_PI*5./8.)  {loc_posy++;}
		else if (secangles[i] < M_PI*7./8.)  {loc_posx--;loc_posy++;}
		else if (secangles[i] < M_PI*9./8.)  {loc_posx--;}
		else if (secangles[i] < M_PI*11./8.) {loc_posx--;loc_posy--;}
		else if (secangles[i] < M_PI*13./8.) {loc_posy--;}
		else if (secangles[i] < M_PI*15./8.) {loc_posx++;loc_posy--;}
		else                                 {loc_posx++;}

		_z[i] = map->GetZ(loc_posx, loc_posy);
	}

	delete secangles;
	return angles_counter;

}