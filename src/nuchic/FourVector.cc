#include <iostream>

#include "nuchic/FourVector.hh"
#include "nuchic/ThreeVector.hh"

nuchic::FourVector::FourVector(const nuchic::ThreeVector& other, const double& E) noexcept
    : vec({other[0], other[1], other[2], E}) {}

const double nuchic::FourVector::M() const noexcept {
    const double m2 = M2();
    if(std::abs(m2) < 1E-6) return 0;
    return sqrt(m2);
}

const double nuchic::FourVector::Theta() const noexcept {
    return atan2(Pt(), vec[2]);
}

const double nuchic::FourVector::Phi() const noexcept {
    const double phi = atan2(vec[1], vec[0]);
    if(phi < 0) return phi + 2*M_PI;
    return phi;
}

const double nuchic::FourVector::Rapidity() const noexcept {
    return 0.5*log((E() + Pz())/(E() - Pz()));
}

const double nuchic::FourVector::DeltaR(const nuchic::FourVector& other) const noexcept {
    const double DEta = Rapidity() - other.Rapidity();
    const double DPhi = Phi() - other.Phi();
    return sqrt(DEta*DEta + DPhi*DPhi);
}

const nuchic::ThreeVector nuchic::FourVector::Vec3() const noexcept {
    return nuchic::ThreeVector(vec[0], vec[1], vec[2]);
}

void nuchic::FourVector::SetVectM(const nuchic::ThreeVector& vec3, const double& mass) noexcept {
    vec[0] = vec3.Px();
    vec[1] = vec3.Py();
    vec[2] = vec3.Pz();
    vec[3] = sqrt(mass*mass + vec3*vec3);
}

nuchic::FourVector nuchic::FourVector::Boost(const nuchic::ThreeVector& beta) noexcept {
    const double beta2 = beta*beta;
    const double gamma = 1.0/sqrt(1.0 - beta2);
    const double betap = beta[0]*vec[0] + beta[1]*vec[1] + beta[2]*vec[2];
    const double gamma2 = beta2 > 0 ? (gamma-1.0)/beta2 : 0.0;

    double pX = vec[0] + gamma2*betap*beta[0]+gamma*beta[0]*vec[3];
    double pY = vec[1] + gamma2*betap*beta[1]+gamma*beta[1]*vec[3];
    double pZ = vec[2] + gamma2*betap*beta[2]+gamma*beta[2]*vec[3];
    double E = gamma*(vec[3] + betap);

    return nuchic::FourVector(pX, pY, pZ, E);
}

nuchic::FourVector nuchic::FourVector::Boost(const double& beta_x, const double& beta_y,
                                             const double& beta_z) noexcept {
    return Boost(nuchic::ThreeVector(beta_x, beta_y, beta_z));
}

nuchic::FourVector nuchic::FourVector::Cross(const nuchic::FourVector& other) const noexcept {
    nuchic::ThreeVector result = this -> Vec3().Cross(other.Vec3());
    return nuchic::FourVector(result, 0);
}

const nuchic::ThreeVector nuchic::FourVector::BoostVector() const noexcept {
    return nuchic::ThreeVector(vec[0]/vec[3], vec[1]/ vec[3], vec[2]/vec[3]);
}

nuchic::FourVector& nuchic::FourVector::operator+=(const nuchic::FourVector& other) noexcept {
    vec[0] += other.vec[0];
    vec[1] += other.vec[1];
    vec[2] += other.vec[2];
    vec[3] += other.vec[3];

    return *this;
}

nuchic::FourVector& nuchic::FourVector::operator-=(const nuchic::FourVector& other) noexcept {
    vec[0] -= other.vec[0];
    vec[1] -= other.vec[1];
    vec[2] -= other.vec[2];
    vec[3] -= other.vec[3];

    return *this;
}

nuchic::FourVector& nuchic::FourVector::operator*=(const double& scale) noexcept {
    vec[0] *= scale;
    vec[1] *= scale;
    vec[2] *= scale;
    vec[3] *= scale;

    return *this;
}

nuchic::FourVector& nuchic::FourVector::operator/=(const double& scale) {
    vec[0] /= scale;
    vec[1] /= scale;
    vec[2] /= scale;
    vec[3] /= scale;

    return *this;
}

double nuchic::FourVector::operator*(const nuchic::FourVector& other) const noexcept {
    return vec[3]*other.vec[3] 
        - (vec[0]*other.vec[0] + vec[1]*other.vec[1] + vec[2]*other.vec[2]);
}

nuchic::FourVector nuchic::FourVector::operator-() const noexcept {
   return nuchic::FourVector(-vec[0],-vec[1],-vec[2],-vec[3]);
}

nuchic::FourVector nuchic::FourVector::operator+() const noexcept {
    return nuchic::FourVector(*this);
}

nuchic::FourVector nuchic::FourVector::operator*(const double& scale) const noexcept {
    return nuchic::FourVector(*this)*=scale;
}

nuchic::FourVector nuchic::FourVector::operator/(const double& scale) const {
    return nuchic::FourVector(*this)/=scale;
}

nuchic::FourVector nuchic::FourVector::operator+(const FourVector& other) const noexcept {
    return nuchic::FourVector(*this)+=other;
}

nuchic::FourVector nuchic::FourVector::operator-(const FourVector& other) const noexcept {
    return nuchic::FourVector(*this)-=other;
}

bool nuchic::FourVector::operator==(const nuchic::FourVector& other) const noexcept {
    return vec==other.vec;
}

const std::string nuchic::FourVector::ToString() const noexcept {
    return "FourVector(" + std::to_string(vec[0]) + ", " + std::to_string(vec[1])
        + ", " + std::to_string(vec[2]) + ", " + std::to_string(vec[3]) + ")";
}

std::ostream& nuchic::operator<<(std::ostream& os, const nuchic::FourVector& vec) {
    os << "FourVector(" << vec.Px() << ", " << vec.Py() << ", " << vec.Pz() << ", " << vec.E() << ")";
    return os;
}

std::istream& nuchic::operator>>(std::istream& is, nuchic::FourVector& vec) {
    std::string head(11, ' '), sep1(1, ' '), sep2(1, ' '),
        sep3(1, ' '), tail(1, ' ');
    double px, py, pz, e;
    is.read(&head[0], 11);
    is >> px;
    is.read(&sep1[0], 1);
    is >> py;
    is.read(&sep2[0], 1);
    is >> pz;
    is.read(&sep3[0], 1);
    is >> e;
    is.read(&tail[0], 1);
    if(head == "FourVector(" &&
       sep1 == "," && sep2 == "," && 
       sep3 == "," && tail == ")") 
        vec = nuchic::FourVector(px, py, pz, e);
    return is;
}

nuchic::FourVector nuchic::operator*(const double& s, const nuchic::FourVector& v) noexcept {
    return v*s;
}
