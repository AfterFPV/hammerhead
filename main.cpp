#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>

using namespace std;

#define SPACE_SIZE 128

using namespace std;


class AstrObject {
	protected:
		int x, y;
		int dmg;
		int speed, range;

	public:
		void tick();
		int get_position();
	private: 
		int update_position();
		int attack();
};


class Ship: public AstrObject {
	
	protected:
	int upgrades;
 	
};

void AstrObject::tick() {

    cout << "huh";

}

int AstrObject::get_position() {

	cout << x << y;

    return 0;
}

class Space {
    int two_d[SPACE_SIZE][SPACE_SIZE];
    list<AstrObject> objs;
    public:
        void tick();
};

int main(int argc, char **argv)
{
    cout << "hello" << endl;

    Space one;
    Ship intrepid;

    return 0;
}
