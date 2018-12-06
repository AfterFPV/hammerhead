#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>

using namespace std;

#define SPACE_SIZE 32

using namespace std;


class AstrObject {
	protected:
		int x, y;
		int dmg;
		int speed, range;

	public:
        AstrObject(int x1, int y1, int dmg1, int speed1, int range1)
        {

            //cout << "x = " << x1 << ", y = " << y1 << endl;
            x = x1;
            y = y1;
            dmg = dmg1;
            speed = speed1;
            range = range1;
            //cout << "constructor" << endl;
        }
		int get_X();
        int get_Y();
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

class Ship: public AstrObject {
	
	protected:
	    int upgrades;
    public:
        Ship(int x, int y, int dmg, int speed, int range): AstrObject(x, y, dmg, speed, range) {
        }
};

class Space {
    char two_d[SPACE_SIZE][SPACE_SIZE];
    list<AstrObject> objs;
    public:
        void tick();
        void add_object(AstrObject obj);
        void print();
        Space() {
            for (int i = 0; i < SPACE_SIZE; i++) {
                for (int j = 0; j < SPACE_SIZE; j++) {
                    two_d[i][j]= '.';
                }
            }
        }
};

void Space::tick() {

	cout << "mawp" << endl;
}

void Space::add_object(AstrObject obj) {
	
    int x, y;
    
    objs.push_back(obj);
    
    x = obj.get_X();
    y = obj.get_Y();
    
    cout << "x = " << x << ", y = " << y << endl;
    two_d[x][y] = 'x';

}

void Space::print() {
	
    for (int i = 0; i < SPACE_SIZE; i++) {
        for (int j = 0; j < SPACE_SIZE; j++) {
            cout << two_d[i][j] << " ";
        }
        cout << endl;
    }

}

int main(int argc, char **argv)
{
    cout << "hello" << endl;

    Space one;
    AstrObject intrepid(1,1,100,43,50);
    AstrObject bad_guys(10,10,100,43,50);

    int x, y;
    x = intrepid.get_X();
    y = intrepid.get_Y();
   

    cout << "x = " << x << ", y = " << y << endl;
    
    one.add_object(intrepid);
    
    one.add_object(bad_guys);
    one.tick();
    one.print();

    return 0;
}
