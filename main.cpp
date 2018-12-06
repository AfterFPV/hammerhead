#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <iostream>

using namespace std;

#define SPACE_SIZE 32

using namespace std;


class Coordinate {
    protected:
        float x, y;
    public:
        Coordinate(float x, float y): x(x), y(y) {
        }
}

class AstrObject {
	protected:
		int x, y;
        int dmg, range;
		int speed;
        int size;

	public:
        AstrObject(int x, int y, int dmg, int speed, int range, int size) :
        x(x), y(y), dmg(dmg), speed(speed), range(range), size(size) {
        }

		int get_X();
        int get_Y();
        int get_size();
	private: 
		int update_position();
		int attack();
};

int AstrObject::get_X() {

    return x;
}

int AstrObject::get_Y() {

    return y;
}

int AstrObject::get_size() {

    return size;
}

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

        x = it->get_X();
        y = it->get_Y();
       
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

        x = it->get_X();
        y = it->get_Y();
        size = it->get_size();
       
        two_d[x][y] = 'x';

        for (int i = x; i < x + size; i++) {
            for (int j = y; j < y + size; j++) {
                two_d[i][j] = 'x';
            }
        }
    }
}

void Space::add_center(AstrObject obj) {


}





int main(int argc, char **argv)
{
    cout << "hello" << endl;

    Space one;
    AstrObject intrepid(1,1,100,43,50,1);
    AstrObject bad_guys(10,10,100,43,50,1);
    AstrObject earth(14,14,0,0,0,4);
    AstrObject moon(20,10,0,0,0,2);
    
    one.add_object(intrepid);
    one.add_object(bad_guys);
    one.add_object(earth);
    one.add_object(moon);



    one.print_map();
    one.tick();
    one.print_list();
    one.print_map();

    return 0;
}
