#ifndef MODEL_STABILITY
#define MODEL_STABILITY

#include<armadillo>

namespace NsModelStability {

class ModelStability {

    const int r;
    arma::mat stabilityMatrix;

public:
    ModelStability(const int r);
    bool isModelStable(const arma::vec & model);
};

}

#endif