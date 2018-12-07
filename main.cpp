#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iterator>
#include <iostream>

using namespace std;

#define SPACE_SIZE 32

using namespace std;


class Coord {
    protected:
        float x, y;
    public:
        Coord(float x, float y): x(x), y(y) {
        }
        int get_intX();
        int get_intY();
        float get_floatX();
        float get_floatY();
};

int Coord::get_intX() {
    return (int)x;
}

int Coord::get_intY() {
    return (int)y;
}

float Coord::get_floatX() {
    return x;
}

float Coord::get_floatY() {
    return y;
}



class AstrObject {
	protected:
		Coord pos;
        Coord center;
        float radius, speed;
        float size;
        int dmg, range;

	public:
        AstrObject(Coord pos, Coord center, float radius, float speed, float size, int dmg, int range) :
            pos(pos), center(center), radius(radius), speed(speed), size(size), dmg(dmg), range(range) {
        }

        Coord get_position();
        Coord get_center();
        float get_radius();
        int get_size();
	private: 
		void update_position();
		int attack();
};

Coord AstrObject::get_position() {

    return pos;
}

Coord AstrObject::get_center() {
 
    return center;
}

float AstrObject::get_radius() {

    return radius;
}

int AstrObject::get_size() {

    return size;
}

void AstrObject::update_position()
{


}

/*
class Ship: public AstrObject {
	
	protected:
	    int upgrades;
    public:
        Ship(int x, int y, int dmg, int speed, int range, int size): 
            AstrObject(x, y, dmg, speed, range, size) {
        }
};

class Planet: public AstrObject {
	
	protected:
	    int upgrades;
    public:
        Planet(int x, int y, int dmg, int speed, int range, int size):
            AstrObject(x, y, dmg, speed, range, size) {
        }
};
*/



class Space {
    char two_d[SPACE_SIZE][SPACE_SIZE];
    list<AstrObject> objs;
    public:
        void tick();
        void add_object(AstrObject obj);
        void print_map();
        void print_list();
        Space() {
            blank_map();            
        }
    private:
        void blank_map();
        void draw_map();
        void draw_orbit(AstrObject &obj);
        void add_center(AstrObject);
};

void Space::tick() {

    blank_map();
    draw_map();

	cout << "tick" << endl;
}

void Space::add_object(AstrObject obj) {
	
    objs.push_back(obj);
}

void Space::print_map() {
	
    for (int i = 0; i < SPACE_SIZE; i++) {
        for (int j = 0; j < SPACE_SIZE; j++) {
            cout << two_d[i][j] << " ";
        }
        cout << endl;
    }
}

void Space::print_list() {

    list<AstrObject>::iterator it;
    int x, y;
    
    for (it = objs.begin(); it != objs.end(); it++) {

        //pos = &(it->get_position());
        x = it->get_position().get_intX();
        y = it->get_position().get_intY();
       
        cout << "x = " << x << ", y = " << y << endl;
    }
}

void Space::blank_map() {

    for (int i = 0; i < SPACE_SIZE; i++) {
        for (int j = 0; j < SPACE_SIZE; j++) {
            two_d[i][j]= '.';
        }
    }
}

void Space::draw_map() {

    int x, y, size;
    list<AstrObject>::iterator it;
    
    for (it = objs.begin(); it != objs.end(); it++) {
        
        x = it->get_position().get_intX();
        y = it->get_position().get_intY();
       
        size = it->get_size();
       
        two_d[x][y] = 'x';

        for (int i = x; i < x + size; i++) {
            for (int j = y; j < y + size; j++) {
                two_d[i][j] = 'x';
            }
        }

        draw_orbit((*it));
    }
}

void Space::draw_orbit(AstrObject &obj) {

    float radius;
    const float PI = 3.14159;
    Coord center = obj.get_center();
    int x, y;
    float cent_x, cent_y;
    float tmp_x, tmp_y;
    
    cent_x = center.get_floatX();
    cent_y = center.get_floatY();
    radius = obj.get_radius();

    for (float angle=0; angle<=2*PI; angle+=0.01) {

        tmp_x = cent_x + (radius * cos(angle));
        tmp_y = cent_y + (radius * sin(angle));

        x = (int)tmp_x;
        y = (int)tmp_y;

        two_d[x][y] = '-';
    }

}

void Space::add_center(AstrObject obj) {


}





int main(int argc, char **argv)
{
    cout << "hello" << endl;

    Space one;

    Coord tmp_pos(1,1);
    Coord tmp_cent(15.5,15.5);

    Coord earth_pos(14,14);

    AstrObject intrepid(tmp_pos,tmp_cent,10.0,1.0,1.0,0,0);
    //AstrObject bad_guys(10,10,100,43,50,1);
    AstrObject earth(earth_pos,earth_pos,10.0,1.0,4.0,0,0);
    //AstrObject moon(20,10,0,0,0,2);
    
    //one.add_object(intrepid);
    //one.add_object(bad_guys);
    one.add_object(earth);
    //one.add_object(moon);



    one.print_map();
    one.tick();
    one.print_list();
    one.print_map();

    return 0;
}
