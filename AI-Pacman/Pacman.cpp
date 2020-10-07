#include "Pacman.h"

extern int maze[MSIZE][MSIZE];
extern bool done;

Pacman::Pacman(Point2D* pos)
{
	this->pos = pos;
	this->win = false;
	this->runMonster = new AStar(pos);
	this->lastPosRunMonster = NULL;
}


Pacman::~Pacman()
{
	delete pos;
	delete runMonster;
	delete lastPosRunMonster;
}

void Pacman::setMonsters(int numberOfMonster, Point2D** &monsters)
{
	AStar astar = AStar(pos);
	for (int i = 0; i < numberOfMonster; i++)
	{
		if (astar.run(*(monsters[i])))
			this->monsters.push_back(monsters[i]);
	}
}

void Pacman::run() {
	int minH = MIN_RUN, temp, monsterIndex, numberOfUse = 0;
	while (!done)
	{
		for (int i = 0; i < (int)monsters.size(); i++)
		{
			temp = abs(pos->getX() - monsters.at(i)->getX()) + abs(pos->getY() - monsters.at(i)->getY());// Manhattan Distance
			if (i == 0 || minH > temp)
			{
				minH = temp;
				monsterIndex = i;
			}
		}
		if (minH < MIN_RUN)
		{
			if (numberOfUse <= 0 || lastPosRunMonster == NULL || !(*pos == *lastPosRunMonster))
			{
				if (lastPosRunMonster != NULL)
				{
					delete lastPosRunMonster;
					lastPosRunMonster = NULL;
				}
				numberOfUse = USE_TIME;
				newFromMonsterPoint(monsters.at(monsterIndex));
			}
			if (runFromMonster())
				numberOfUse--;
			else
				numberOfUse = 0;
			lastPosRunMonster = new Point2D(*pos);
		}
		(maze)[pos->getY()][pos->getX()] = PACMAN;
	}
	if (win)
		maze[pos->getY()][pos->getX()] = PACMAN_WIN;
	else
		maze[pos->getY()][pos->getX()] = PACMAN_DIE;
}

bool Pacman::runFromMonster()
{
	Point2D* helper = runMonster->getTop();

	if (helper != NULL)
	{
		this_thread::sleep_for(chrono::microseconds(PACMAN_SLEEP));
		(maze)[pos->getY()][pos->getX()] = SPACE;
		pos->copy(*helper);
		delete helper;
		return true;
	}
	return false;
}

void Pacman::newFromMonsterPoint(Point2D* &monster)
{
	int mX, mY;
	mX = monster->getX();
	mY = monster->getY();
	int posX, posY;
	posX = pos->getX() - mX;
	if (posX != 0)
		posX = posX / abs(posX);
	posY = pos->getY() - mY;
	if (posY != 0)
		posY = posY / abs(posY);
	Point2D pos = Point2D((MSIZE - mX + posX * 10) % MSIZE, (MSIZE - mY + posY * 10) % MSIZE);
	runMonster->run(pos, RUN);
}