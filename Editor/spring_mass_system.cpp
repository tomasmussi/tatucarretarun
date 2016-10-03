#include "spring_mass_system.h"

#include <cassert>

namespace
{
	double calcDist(const Vector2<>& v1, const Vector2<>& v2)
	{
		Vector2<> d = v2;
		d -= v1;
		return d.norm();
	}

	void inellasticallyCollide(std::vector<Vector2<> >& y, SMSStaticState& x,
		int m1, int m2, double distance)
	{
		// gets projections of velocity
		Vector2<> axis = y[m2];
		axis -= y[m1];
		double dist = axis.norm();
		axis.normalize();
		double v1p = axis.dot(y[m1 + y.size() / 2]);
		double v2p = axis.dot(y[m2 + y.size() / 2]);

		// checks if the masses really collide
		if (v1p < v2p)
			return;

		// m1 * vf + m2 * vf = m1 * v01 + m2 * v02
		// (1 / m2) * vf + (1 / m1) * vf = (1 / m2) * v01 + (1 / m1) * v02
		// vf = ((1 / m2) * v01 + (1 / m1) * v02) / ((1 / m1) + (1 / m2))
		double vf = (v1p * x.invMassVec[m2] + v2p * x.invMassVec[m1]) /
			(x.invMassVec[m1] + x.invMassVec[m2]);

		// updates velocities
		Vector2<> dv1 = axis;
		Vector2<> dv2 = axis;
		dv1 *= (vf - v1p);
		dv2 *= (vf - v2p);
		y[m1 + y.size() / 2] += dv1;
		y[m2 + y.size() / 2] += dv2;

		// restores distance
		if (dist < distance)
		{
			Vector2<> dx = axis;
			dx *= (distance - dist) / 2.0;
			y[m1] -= dx;
			y[m2] += dx;
		}
	}

	void handleCollisions(std::vector<Vector2<> >& y, SMSStaticState& x)
	{
		// handles self collisions
		for (size_t i = 0; i < x.collMassesIndices.size(); i++)
		{
			for (size_t j = 0; j < i; j++)
			{
				int m1 = x.collMassesIndices[i];
				int m2 = x.collMassesIndices[j];
				double r1 = x.collMassesRadii[i];
				double r2 = x.collMassesRadii[j];

				if (calcDist(y[m1], y[m2]) >= r1 + r2)
					continue;
				
				inellasticallyCollide(y, x, m1, m2, r1 + r2);
			}
		}
	}

	void calcYPrime(std::vector<Vector2<> >& yPrimeVec,
		const std::vector<Vector2<> >& y,
		SMSStaticState& x,std::vector<Element*>  cohetes,std::vector<Element*>  ruedas)
	{
		// spring forces
		for (size_t i = 0; i < x.springEndPointsVec.size(); i++)
		{
			Vector2<> delta(y[x.springEndPointsVec[i].second]);
			delta -= y[x.springEndPointsVec[i].first];

			double forceMag = - x.springKsVec[i] * 
				(delta.norm() - x.springNormalLengthsVec[i]);

			// quick hack...
			if (fabs(forceMag) * x.springInvStrength[i] > 1.0)
				x.springKsVec[i] = 0.0;

			delta.normalize();
			delta *= forceMag;
			yPrimeVec[x.springEndPointsVec[i].second + y.size() / 2] += delta;

			delta *= -1.0;
			yPrimeVec[x.springEndPointsVec[i].first + y.size() / 2] += delta;
		}

		// viscous drag
		for (size_t i = 0; i < x.invMassVec.size(); i++)
		{
			Vector2<> drag(y[i + y.size() / 2]);
			drag *= x.damping;
			yPrimeVec[i + y.size() / 2] -= drag;
		}	
		
		// dx/dt = v
		for (size_t i = 0; i < yPrimeVec.size() / 2; i++)
			yPrimeVec[i] = y[i + y.size() / 2];

		// dv/dt = force * (1 / mass)
		for (size_t i = 0; i < x.invMassVec.size(); i++)
			yPrimeVec[i + yPrimeVec.size() / 2] *= x.invMassVec[i];

		// adds gravity
		for (size_t i = 0; i < x.invMassVec.size(); i++)
			if (x.invMassVec[i] != 0.0)
				yPrimeVec[i + yPrimeVec.size() / 2] += x.g;
		
	
		unsigned it;
		double force,hip,fx,fy,dx,dy;
		force = 1.5;
		for (it=0; it<cohetes.size();it++){ //para cada una de los cohetes
			Vector2<> m1 = y[cohetes.at(it)->masas.at(cohetes.at(it)->masas.size()-1)];//mp.getPos();
			Vector2<> m2 = y[cohetes.at(it)->masas.at(cohetes.at(it)->masas.size()-2)];//mp.getPos();
			dx = m2.x - m1.x;			
			dy = m2.y - m1.y;			
			hip = sqrt(pow(dx,2) + pow(dy,2));
			fx = force * (dx/hip);
			fy = force * (dy/hip);
			
			for (unsigned i = 0; i <cohetes.at(it)->masas.size();i++)			
				yPrimeVec[cohetes.at(it)->masas.at(i) + yPrimeVec.size() / 2] += Vector2<> (fx,fy);//x.g;
		}
		
		
		force = 1.5;
		double alfa; 
		for (it=0; it<ruedas.size();it++){ //para cada una de los cohetes
			Vector2<> central = y[ruedas.at(it)->getFirst()];
			Vector2<> m2;
			double thisforce;
			for (unsigned i = ruedas.at(it)->getFirst() + 1; i <= ruedas.at(it)->getLast();i++){			
				m2 = y[i];
				dx = m2.x - central.x;			
				dy = m2.y - central.y;
				alfa = atan(dy/dx);
				thisforce = force *  x.invMassVec[i];
				fx = thisforce * cos (alfa);
				fy = thisforce * sin (alfa);
				
				if (dy < 0) fy = -fy;
				yPrimeVec[i + yPrimeVec.size() / 2] +=	Vector2<> (fx,fy);
			}
		}



	}
}

MassProxy::MassProxy(SpringMassSystem& sms, size_t index) : 
SMS_(sms), index_(index)
{
}

double MassProxy::getInvMass() const
{
	return SMS_.x_.invMassVec[index_];
}

const Vector2<>& MassProxy::getPos() const
{
	return SMS_.y_[index_];
}

const Vector2<>& MassProxy::getVel() const
{
	return SMS_.y_[index_ + SMS_.y_.size() / 2];
}

void MassProxy::setVel(const Vector2<>& v)
{
	SMS_.y_[index_ + SMS_.y_.size() / 2] = v;
}

SpringProxy::SpringProxy(SpringMassSystem& sms,  size_t index) :
SMS_(sms), index_(index)
{
}

const Vector2<>& SpringProxy::getStartPos() const
{
	return SMS_.y_[SMS_.x_.springEndPointsVec[index_].first];
}

const Vector2<>& SpringProxy::getEndPos() const
{
	return SMS_.y_[SMS_.x_.springEndPointsVec[index_].second];
}

double SpringProxy::getRelElongation() const
{
	Vector2<> delta = getEndPos();
	delta -= getStartPos();

	return delta.norm() / 
		SMS_.x_.springNormalLengthsVec[index_] - 1.0; //estos en 0
}

double SpringProxy::getK() const
{
	return SMS_.x_.springKsVec[index_];
}

SpringMassSystem::SpringMassSystem(const Config& config)
{
	x_.g = config.read<Vector2<> >("SMS.G");
	x_.damping = config.read<double>("SMS.Damping");
}

int SpringMassSystem::addMass(double invMass, const Vector2<>& pos)
{
	x_.invMassVec.push_back(invMass);
	y_.resize(y_.size() + 2);
	y_[y_.size() / 2 - 1] = pos;

	return x_.invMassVec.size() - 1;
}

int SpringMassSystem::addCollMass(double invMass, const Vector2<>& pos,
								  double radius)
{
	int i = addMass(invMass, pos);
	x_.collMassesIndices.push_back(i);
	x_.collMassesRadii.push_back(radius);
	return i;
}

void SpringMassSystem::fixMass(int pos)
{
	x_.invMassVec.at(pos)=0;
}

int SpringMassSystem::addSpring(int startMass, int endMass, double k,
								double relElongation, double invStrength)
{
	x_.springEndPointsVec.push_back(
		std::make_pair(startMass, endMass));
	x_.springKsVec.push_back(k);

	Vector2<> delta(y_[endMass]);
	delta -= y_[startMass];

	x_.springNormalLengthsVec.push_back(
		delta.norm() / (1.0 + relElongation));

	x_.springInvStrength.push_back(invStrength);

	return x_.springKsVec.size() - 1;
}

size_t SpringMassSystem::getNumMasses() const
{
	return x_.invMassVec.size();
}

size_t SpringMassSystem::getNumSprings() const
{
	return x_.springKsVec.size();
}

MassProxy SpringMassSystem::getMassProxy(size_t index)
{
	return MassProxy(*this, index);
}

SpringProxy SpringMassSystem::getSpringProxy(size_t index)
{
	return SpringProxy(*this, index);
}

void SpringMassSystem::setMapa(Mapa* mapa){
	this->mapa = mapa;
}

Mapa* SpringMassSystem::getMapa(){
	return this->mapa;
}

bool SpringMassSystem::sameElement(unsigned first,unsigned second){
	unsigned ini,fin;	
	for (unsigned i=0; i<manager.vectoresElementos.size();i++)
		for (unsigned j=0; j<manager.vectoresElementos.at(i)->size();j++)
		{	
			ini =(manager.vectoresElementos.at(i)->at(j))->getFirst();
			fin =(manager.vectoresElementos.at(i)->at(j))->getLast();
			if ( (first>=ini && first<=fin) && (second>=ini && second<=fin) )
				return true;
		}
	return false;
}

void SpringMassSystem::unifyMass(int first,int second){ 
	for (unsigned i=0; i<manager.vectoresElementos.size();i++)  //en los elementos reemplaza las masas unificadas
		for (unsigned j=0; j<manager.vectoresElementos.at(i)->size();j++)
			for (unsigned k=0; k<(manager.vectoresElementos.at(i)->at(j))->masas.size();k++){
				if ( ((manager.vectoresElementos.at(i)->at(j))->masas.at(k) ) == first ){
					((manager.vectoresElementos.at(i)->at(j))->masas.at(k) ) = second;
					}
			}
	
	for (unsigned i=0; i<x_.springEndPointsVec.size();i++){
		if (x_.springEndPointsVec.at(i).first == first) {
			x_.springEndPointsVec.at(i).first = second;
		}	
		if (x_.springEndPointsVec.at(i).second == first) {
			x_.springEndPointsVec.at(i).second = second;
		}	
	}
}

std::vector<int> SpringMassSystem::getVecEnganches(){
	return enganches;
}

void SpringMassSystem::addEnganche(int enganche){
	enganches.push_back(enganche);
}

bool estanCerca(Vector2<> pos1,Vector2<> pos2){

	double modulo,dx,dy;
	dx = pos1.x - pos2.x;
	dy = pos2.y - pos1.y;
	
	modulo = sqrt(pow(dx,2)+pow(dy,2));
	return (modulo <= 0.04);
	
}

void SpringMassSystem::checkRoutine(){
	std::vector<int> enganches = getVecEnganches();
	
	for (size_t i = 0; i< enganches.size() - 1; i++){
		for (size_t j=i+1; j<enganches.size(); j++){
			MassProxy mp2 = getMassProxy(enganches.at(j));
			Vector2<> pos2 = mp2.getPos();
			MassProxy mp = getMassProxy(enganches.at(i));
			Vector2<> pos1 = mp.getPos();
			if (estanCerca(pos1,pos2)){
				if (mp.getInvMass() != 0){
					unifyMass(enganches.at(i),enganches.at(j));	//vuelan i
					y_.at(enganches.at(j)).x=y_.at(enganches.at(i)).x;			
					y_.at(enganches.at(j)).y=y_.at(enganches.at(i)).y;	
					y_.at(enganches.at(i)).x=rand()*12;			
					y_.at(enganches.at(i)).y=5;
					}
				else {
					unifyMass(enganches.at(j),enganches.at(i));
					y_.at(enganches.at(i)).x=y_.at(enganches.at(j)).x;			
					y_.at(enganches.at(i)).y=y_.at(enganches.at(j)).y;	
					y_.at(enganches.at(j)).x=rand()*12;			
					y_.at(enganches.at(j)).y=5;					
					}
			}
		}

	} 
 	int mas1, mas2;
 
  	for (size_t i = 0; i< x_.springNormalLengthsVec.size();i++){
	 	mas1 = x_.springEndPointsVec.at(i).first;
	 	mas2 = x_.springEndPointsVec.at(i).second;
	 
		Vector2<> delta(y_[mas2]);
		delta -= y_[mas1];

		x_.springNormalLengthsVec[i] = (delta.norm()); 
 	}		 
}
double SpringMassSystem::getPotentialEnergy() const
{
	double pe = 0.0;

	Vector2<> deltaVec;
	double deltaNorm;

	// accumulates springs' potential energy
	for (size_t i = 0; i < x_.springEndPointsVec.size(); i++)
	{
		deltaVec = y_[x_.springEndPointsVec[i].second];
		deltaVec -= y_[x_.springEndPointsVec[i].first];
		deltaNorm = deltaVec.norm() - x_.springNormalLengthsVec[i];
		pe += 0.5 * x_.springKsVec[i] * deltaNorm * deltaNorm;
	}

	// accumulates masses' potential energy
	for (size_t i = 0; i < x_.invMassVec.size(); i++)
		if (x_.invMassVec[i] != 0.0)
			pe += -y_[i].dot(x_.g) / x_.invMassVec[i];

	return pe;
}

double SpringMassSystem::getKineticEnergy() const
{
	double ke = 0.0;

	// accumulates masses' kinetic energy
	for (size_t i = 0; i < y_.size() / 2; i++)
		if (x_.invMassVec[i] != 0.0)
			ke += (0.5 / x_.invMassVec[i]) * y_[i + y_.size() / 2].sqNorm();
		else
			continue;

	return ke;
}

double SpringMassSystem::getEnergy() const
{
	return getPotentialEnergy() + getKineticEnergy();
}

void SpringMassSystem::eulerStep(double dt)
{
	std::vector<Vector2<> > yPrimeVec(y_.size());

	// calculates derivative of system state (y prime)
	calcYPrime(yPrimeVec, y_, x_,manager.getCohetes(),manager.getRuedas());

	// update positions and velocities
	for (size_t i = 0; i < y_.size(); i++)
	{
		Vector2<> delta = yPrimeVec[i];
		delta *= dt;
		y_[i] += delta;
	}

	// handles collisions
	handleCollisions(y_, x_);
}

void SpringMassSystem::rk4Step(double dt)
{
	std::vector<Vector2<> > k1(y_.size());
	std::vector<Vector2<> > k2(y_.size());
	std::vector<Vector2<> > k3(y_.size());
	std::vector<Vector2<> > k4(y_.size());

	std::vector<Vector2<> > yTmp(y_.size());

	// gets k1
	calcYPrime(k1, y_, x_,manager.getCohetes(),manager.getRuedas());

	// gets argument to calculate k2
	for (size_t i = 0; i < yTmp.size(); i++)
	{
		Vector2<> delta = k1[i];
		delta *= dt / 2.0;
		yTmp[i] = y_[i];
		yTmp[i] += delta;
	}

	// gets k2
	calcYPrime(k2, yTmp, x_,manager.getCohetes(),manager.getRuedas());

	// gets argument to calculate k3
	for (size_t i = 0; i < yTmp.size(); i++)
	{
		Vector2<> delta = k2[i];
		delta *= dt / 2.0;
		yTmp[i] = y_[i];
		yTmp[i] += delta;
	}

	// gets k3
	calcYPrime(k3, yTmp, x_,manager.getCohetes(),manager.getRuedas());

	// gets argument to calculate k4
	for (size_t i = 0; i < yTmp.size(); i++)
	{
		Vector2<> delta = k3[i];
		delta *= dt;
		yTmp[i] = y_[i];
		yTmp[i] += delta;
	}

	// gets k4
	calcYPrime(k4, yTmp, x_,manager.getCohetes(),manager.getRuedas());

	// updates state
	for (size_t i = 0; i < y_.size(); i++)
	{
		Vector2<> delta = k2[i];
		delta += k3[i];
		delta *= 2.0;
		delta += k1[i];
		delta += k4[i];

		delta *= dt / 6.0;

		y_[i] += delta;
	}

	// handles collisions
	handleCollisions(y_, x_);
}

void SpringMassSystem::debugPrint(std::ostream& os) const
{
	for (size_t i = 0; i < x_.invMassVec.size(); i++)
	{
		os << "Mass " << i << "\n";
		os << "\tInvMass: " << x_.invMassVec[i] << "\n";
		os << "\tPos: " << y_[i] << "\n";
		os << "\tVel: " << y_[i + y_.size() / 2] << "\n";
	}

	for (size_t i = 0; i < x_.springEndPointsVec.size(); i++)
	{
		os << "Spring " << i << "\n";
		os << "\tStart point: " << x_.springEndPointsVec[i].first << "\n";
		os << "\tEnd point: " << x_.springEndPointsVec[i].second << "\n";
		os << "\tK: " << x_.springKsVec[i] << "\n";
		os << "\tNormalLength: " << x_.springNormalLengthsVec[i] << "\n";
	}
} 
ElementManager* SpringMassSystem::getElementManager(){
	return &manager;
}

