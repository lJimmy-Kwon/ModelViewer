#ifndef MATRIX_H
#define MATRIX_H
#include <QMatrix4x4>
#include <assimp/scene.h>

class zero4x4 :public QMatrix4x4{
    public:
        zero4x4(){
            this->fill(0);
        }

        zero4x4(QMatrix4x4& mat){
            for(uint i = 0 ; i < 4 ; i++){
                for(uint j = 0 ; j < 4 ; j++){
                    (*this)(i, j) = mat(i, j);
                }
            }
        }

        QMatrix4x4* operator=(QMatrix4x4 mat){
            for(uint i = 0 ; i < 4 ; i++){
                for(uint j = 0 ; j < 4 ; j++){
                    (*this)(i, j) = mat(i, j);
                }
            }

            return this;
        }
};


inline QMatrix4x4 qMat44_aiMat44(aiMatrix4x4& from){
    QMatrix4x4 to;

    for(uint i = 0 ; i < 4 ; i++){
        for(uint j = 0 ; j < 4 ; j++){
            to(i, j) = from[i][j];
        }
    }

    return to;
}

inline QMatrix4x4 qMat44_aiMat44( const aiMatrix4x4& from){
    QMatrix4x4 to;

    for(uint i = 0 ; i < 4 ; i++){
        for(uint j = 0 ; j < 4 ; j++){
            to(i, j) = from[i][j];
        }
    }

    return to;
}


inline aiMatrix4x4 aiMat44_qMat44(QMatrix4x4& from){
    aiMatrix4x4 to;

    for(uint i = 0 ; i < 4 ; i++){
        for(uint j = 0 ; j < 4 ; j++){
            to[i][j] = from(i,j);
        }
    }

    return to;

}

inline QMatrix4x4 qMat44_aiMat33(const aiMatrix3x3& AssimpMatrix)
{
    QMatrix4x4 m;

    m(0, 0) = AssimpMatrix.a1; m(0, 1) = AssimpMatrix.a2; m(0, 2) = AssimpMatrix.a3; m(0, 3) = 0.0f;
    m(1, 0) = AssimpMatrix.b1; m(1, 1) = AssimpMatrix.b2; m(1, 2) = AssimpMatrix.b3; m(1, 3) = 0.0f;
    m(2, 0) = AssimpMatrix.c1; m(2, 1) = AssimpMatrix.c2; m(2, 2) = AssimpMatrix.c3; m(2, 3) = 0.0f;
    m(3, 0) = 0.0f           ; m(3, 1) = 0.0f           ; m(3, 2) = 0.0f           ; m(3, 3) = 1.0f;
}



#endif // MATRIX_H
