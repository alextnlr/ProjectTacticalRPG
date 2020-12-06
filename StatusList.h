#pragma once

typedef struct status_struct {
	int power;
	int timer;
} Status;

typedef enum {Grass, Forest, Bastion, Ruin, River} TerrainEffect;

class StatusList
{
public:
	StatusList();
	void resetAll();
	void decreaseTimerAll();
	int getShield();
	int getPrecision();
	int getArcana();
	void setShieldUp(int power, int timer);
	void setShieldDown(int power, int timer);
	void setTerrain(int terrain);
private:
	Status m_shieldUp;
	Status m_shieldDown;
	Status m_precisionUp;
	Status m_precisionDown;
	Status m_arcanaUp;
	Status m_arcanaDown;

	TerrainEffect m_terrain;
};

