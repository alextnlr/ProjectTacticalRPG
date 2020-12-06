#include "StatusList.h"


StatusList::StatusList()
{
	m_shieldUp = {0, 0};
	m_shieldDown = {0, 0};
	m_precisionUp = {0, 0};
	m_precisionDown = {0, 0};
	m_arcanaUp = {0, 0};
	m_arcanaDown = {0, 0};

	m_terrain = Grass;
}

void StatusList::resetAll()
{
	if (m_shieldUp.timer == 0) { m_shieldUp.power = 0; }
	if (m_shieldDown.timer == 0) { m_shieldDown.power = 0; }
	if (m_precisionUp.timer == 0) { m_precisionUp.power = 0; }
	if (m_precisionDown.timer == 0) { m_precisionDown.power = 0; }
	if (m_arcanaUp.timer == 0) { m_arcanaUp.power = 0; }
	if (m_arcanaDown.timer == 0) { m_arcanaDown.power = 0; }
}

void StatusList::decreaseTimerAll()
{
	if (m_shieldUp.timer - 1 >= 0) { m_shieldUp.timer--; }
	if (m_shieldDown.timer - 1 >= 0) { m_shieldDown.timer--; }
	if (m_precisionUp.timer - 1 >= 0) { m_precisionUp.timer--; }
	if (m_precisionDown.timer - 1 >= 0) { m_precisionDown.timer--; }
	if (m_arcanaUp.timer - 1 >= 0) { m_arcanaUp.timer--; }
	if (m_arcanaDown.timer - 1 >= 0) { m_arcanaDown.timer--; }
}

int StatusList::getShield()
{
	if (m_terrain == Bastion || m_terrain == Ruin || m_terrain == Forest)
	{
		return m_shieldUp.power - m_shieldDown.power + 2;
	}
	else if (m_terrain == River)
	{
		return m_shieldUp.power - m_shieldDown.power - 2;
	}
	return m_shieldUp.power - m_shieldDown.power;
}

int StatusList::getPrecision()
{
	if (m_terrain == Bastion)
	{
		return m_precisionUp.power - m_precisionDown.power + 2;
	}
	else if (m_terrain == Forest || m_terrain == River || m_terrain == Ruin)
	{
		return m_precisionUp.power - m_precisionDown.power - 2;
	}
	return m_precisionUp.power - m_precisionDown.power;
}

int StatusList::getArcana()
{
	if (m_terrain == Ruin)
	{
		return m_arcanaUp.power - m_arcanaDown.power + 2;
	}
	return m_arcanaUp.power - m_arcanaDown.power;
}

void StatusList::setShieldUp(int power, int timer)
{
	m_shieldUp.power = power;
	m_shieldUp.timer = timer;
}

void StatusList::setShieldDown(int power, int timer)
{
	m_shieldDown.power = power;
	m_shieldDown.timer = timer;
}

void StatusList::setTerrain(int terrain)
{
	switch (terrain)
	{
	case 0:
		m_terrain = Grass;
		break;
	case 1:
		m_terrain = Forest;
		break;
	case 2:
		m_terrain = Bastion;
		break;
	case 3:
		m_terrain = Ruin;
		break;
	case 4:
	case 5:
		m_terrain = River;
		break;
	default:
		break;
	}
}