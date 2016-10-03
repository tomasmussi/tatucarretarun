#ifndef SPRING_MASS_SYSTEM_H
#define SPRING_MASS_SYSTEM_H
#include <algorithm>
#include "config.h"
#include "vector2.h"
#include "hiloCron.h"
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
	/*Devuelve el valor de una masa determinada*/
	double getInvMass() const;
	
	/*Devuelve la posicion de una masa determinada*/
	const Vector2<>& getPos() const;
	
	/*Devuelve la posicion de la masa*/
	const Vector2<>& getVel() const;
	
	/*Setea la velocidad de dicha masa*/
	void setVel(const Vector2<>& v);
};

class SpringProxy
{
	friend class SpringMassSystem;

	SpringMassSystem& SMS_;
	size_t index_;

	SpringProxy(SpringMassSystem& sms, size_t index);

public:
	/*Devuleve la posición de la masa en 
	la que comienza el resorte*/
	const Vector2<>& getStartPos() const;
	
	/*Devuelve la posicion de la masa hasta
	donde llega el resorte*/
	const Vector2<>& getEndPos() const;
	
	/*Devuelve la elongacion del resorte
	en el momento en que se llama*/
	double getRelElongation() const;
	
	/*Devuelve la constante del resorte*/
	double getK() const;
};

struct SMSStaticState
{
	// Masses data
	std::vector<double> invMassVec;

	// Springs data
	/*Se alamcenan los valores estaticos del sistema:
	 . masas que une cada resorte
	 . constantes de los resortes
	 . elongaciones iniciales de los resortes
	 . resistencia de los resortes
	 */
	std::vector<std::pair<int, int> > springEndPointsVec;
	std::vector<double> springKsVec;
	std::vector<double> springNormalLengthsVec;
	std::vector<double> springInvStrength;

	// Gravity
	Vector2<> g;

	// Viscous damping
	double damping;

	// Collidable masses info
	/*Almacena las masas que colisionan y
	sus respectivos radios de colision*/
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
	long rocketTime;
	HiloCron* cronometro;
public:
	SpringMassSystem(const Config& config);
	
	/*Agrega una masa al sistema*/
	int addMass(double invMass, const Vector2<>& pos);
	
	/*Agrega una masa que colisiona al sistema*/
	int addCollMass(double invMass, const Vector2<>& pos, double radius);
	
	/*Agrega un resorte al sistema*/
	int addSpring(int startMass, int endMass, double k,
		double relElongation = 0.0, double invStrength = 0.0);
		
	/*Fija una masa determinada*/
	void fixMass(int pos);
	
	/*Devuelven los proxys de masas y resortes
	Se obtienen resortes y masas específicos determinados
	por el parametro index
	*/
	MassProxy getMassProxy(size_t index);
	SpringProxy getSpringProxy(size_t index);

	/*Devuelve la cantidad de masas y resortes*/
	size_t getNumMasses() const;
	size_t getNumSprings() const;

	double getPotentialEnergy() const;
	double getKineticEnergy() const;
	double getEnergy() const;

	void eulerStep(double dt);
	void rk4Step(double dt);

	void debugPrint(std::ostream& os) const;

	/*Unifica las dos masas pasados por parametros
	esta funcion se utiliza para los enganches*/
	void unifyMass(int first,int second);
	
	/*Realiza las verificaciones de proximidad
	para unificar las masas*/
	void checkRoutine();
	
	/*Devuelve un puntero al ElementManager*/
	ElementManager* getElementManager();
	
	/*Devuelve el vector con las masas que son
	enganches, con estos verifica la proximidad*/
	std::vector<int> getVecEnganches();
	
	/*agrega una masa al vector de enganches*/
	void addEnganche(int enganche);
	
	/*Alamacena una referencia al mapa editado
	para instanciar el sistema en base a este*/
	void setMapa(Mapa* mapa);

	void setCronometro(HiloCron *cronometro);
	/*Setea el tiempo de cohetes*/
	void setRocketTime(long time);		
	/*Devuelve la referencia al mapa*/
	Mapa* getMapa();
};

#endif
