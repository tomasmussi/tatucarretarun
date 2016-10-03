#ifndef SPRING_MASS_SYSTEM_H
#define SPRING_MASS_SYSTEM_H
#include <algorithm>
#include "config.h"
#include "vector2.h"
#include "elementManager.h"
#include <utility>
#include <vector>
#include <list>
#include "Mapa.h"

class SpringMassSystem;

class MassProxy
{
	friend class SpringMassSystem;

	SpringMassSystem& SMS_;
	size_t index_;

	MassProxy(SpringMassSystem& sms, size_t index);

public:
	double getInvMass() const;
	const Vector2<>& getPos() const;
	const Vector2<>& getVel() const;
	void setVel(const Vector2<>& v);
};

class SpringProxy
{
	friend class SpringMassSystem;

	SpringMassSystem& SMS_;
	size_t index_;

	SpringProxy(SpringMassSystem& sms, size_t index);

public:
	const Vector2<>& getStartPos() const;
	const Vector2<>& getEndPos() const;
	double getRelElongation() const;
	double getK() const;
};

struct SMSStaticState
{
	// Masses data
	std::vector<double> invMassVec;

	// Springs data
	std::vector<std::pair<int, int> > springEndPointsVec;
	std::vector<double> springKsVec;
	std::vector<double> springNormalLengthsVec;
	std::vector<double> springInvStrength;

	// Gravity
	Vector2<> g;

	// Viscous damping
	double damping;

	// Collidable masses info
	std::vector<int> collMassesIndices;
	std::vector<double> collMassesRadii;
};

class SpringMassSystem
{
	// System dynamic state (the positions are stored in the first half and 
	// velocities in the other half)
	std::vector<Vector2<> > y_;

	// System static state (the part of the state that doesn't change)
	SMSStaticState x_;
	
	// Friends 
	friend class MassProxy;
	friend class SpringProxy;

	ElementManager manager;
	Mapa* mapa;
	std::vector<int> enganches;
	
public:
	SpringMassSystem(const Config& config);
	
	int addMass(double invMass, const Vector2<>& pos);
	int addCollMass(double invMass, const Vector2<>& pos, double radius);
	int addSpring(int startMass, int endMass, double k,
		double relElongation = 0.0, double invStrength = 0.0);
	void fixMass(int pos);
	MassProxy getMassProxy(size_t index);
	SpringProxy getSpringProxy(size_t index);

	size_t getNumMasses() const;
	size_t getNumSprings() const;

	double getPotentialEnergy() const;
	double getKineticEnergy() const;
	double getEnergy() const;

	void eulerStep(double dt);
	void rk4Step(double dt);

	void debugPrint(std::ostream& os) const;

	bool sameElement(unsigned first,unsigned second);
	void unifyMass(int first,int second);
	void checkRoutine();
	ElementManager* getElementManager();
	std::vector<int> getVecEnganches();
	void addEnganche(int enganche);
	
	void setMapa(Mapa* mapa);
	Mapa* getMapa();
};

#endif
