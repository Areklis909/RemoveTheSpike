#include <armadillo>


using namespace arma;

int main() {
    mat m(5,5);
    mat o(5,5, fill::ones);

    m(1,1, size(2,2)) = o(0,0, size(2,2));
    m(0, 0, size(5, 1)) = m(0, 1, size(5, 1));
    m.print("macierz m: ");
}
