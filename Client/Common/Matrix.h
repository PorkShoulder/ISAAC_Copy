#pragma once
#include "Vector4D.h"

//행렬
//게임에서 행렬을 많이 써요.
//행렬 왜 씀?? 여러 변환(크기, 이동, 회전)을 하나의 연산으로 묶어서 처리하려고

/*
[4 * 4]
1 2 3 4
5 6 7 8
1 2 3 4
5 6 7 8

단위 행렬 : 대각선이 1, 나머지는 0
1 0 0 0
0 1 0 0
0 0 1 0
0 0 0 1
(항등행렬)

행렬의 곱 ---> 교환법칙 X

전치행렬
행렬의 열과 행을 뒤집는걸 전치행렬

크기 행렬
x 0 0 0
0 y 0 0
0 0 z 0
0 0 0 1

X축 회전 행렬
1   0   0   0
0  cos -sin 0
0  sin  cos 0
0   0   0   1

Y축 회전 행렬
cos  0   sin 0
0    1   0   0
-sin 0   cos 0
0    0   0   1

Z축 회전 행렬
cos -sin    0   0
sin cos     0   0
0   0       1   0
0   0       0   1

최종 회전 행렬 : x축 회전행렬 * Y축 회전 행렬 * Z 회전 행렬

이동 행렬
1   0   0   0
0   1   0   0
0   0   1   0
x   y   z   1

월드행렬
--> 크기 * 자전 * 이동 * 부모행렬
    --> 부모 행렬은 상속받은 클래스가 아닌 나에게 영향을 줄 수 있는 다른 객체

오일러 각
3D 회전을 x, y, z 축 가각의 회전의 값으로 표현
짐벌락이 발생할 수 있다.
짐벌락 : 한 축이 90도 회전하게 되면 나머지 두 축이 겹치게 되서
회전이 제대로 되지 않는걸 의미합니다.

쿼터니언(사원수)
숫자가 4개
x축 회전 : pitch
y축 회전 : yaw
z축 회전 : roll

x, y, z, 회전양

*/

//union
//내부 변수끼리 메모리를 공유

//_declspec(align(16))
//내부 데이터를 16바이트의 배수에 맞게 강제로 맞추라는 명령을
//컴파일러에게 알려주는 키워드
//why? 16byte
//gpu는 16바이트 단위로 데이트를 읽어요.
_declspec(align(16)) union FMatrix
{
    DirectX::XMMATRIX _m;

    struct
    {
        float _11, _12, _13, _14;
        float _21, _22, _23, _24;
        float _31, _32, _33, _34;
        float _41, _42, _43, _44;
    };

    FVector4D _v[4] = {};

    FMatrix();
    FMatrix(const FMatrix& m);
    FMatrix(FMatrix&& m) noexcept;
    FMatrix(const DirectX::XMMATRIX& m);
    FMatrix(DirectX::XMMATRIX&& m) noexcept;
    FMatrix(const FVector4D v[4]);

    FVector4D& operator[] (int32 index);
    
    const FMatrix& operator = (const FMatrix& m);
    const FMatrix& operator = (const DirectX::XMMATRIX& m);
    const FMatrix& operator = (const FVector4D v[4]);

    FMatrix operator * (const FMatrix& m) const;
    FMatrix operator * (const DirectX::XMMATRIX& m) const;

    //단위행렬을 만드는 함수
    void Indentity();

    //전치 행렬
    void Transpose();

    //역행렬
    void Inverse();

    //크기
    void Scaling(const struct FVector3D& v);
    void Scaling(float x, float y, float z);
    void Scaling(const FVector2D& v);
    void Scaling(float x, float y);


    //회전
    void Rotation(const FVector3D& v);
    void Rotation(float x, float y, float z);
    void RotationX(float x);
    void RotationY(float y);
    void RotationZ(float z);
    void RotationAxis(const FVector3D& axis, float angle);

    //이동
    void Translation(const FVector3D& v);
    void Translation(float x, float y, float z);
    void Translation(const FVector2D& v);
    void Translation(float x, float y);

    void ExtractScale(OUT FVector3D& outVal) const;
    void ExtractPosition(OUT FVector3D& outVal) const;
    void ExtractEuler(OUT FRotator& outVal) const;

    FVector3D ExtractScale() const;
    FVector3D ExtractPosition() const;
    FVector3D ExtractEuler() const;
};


/*
[A]                 [B]
1 | 2 | 3 | 4 |  1 | 2 | 3 | 4 |
5 | 6 | 7 | 8 |  5 | 6 | 7 | 8 |
1 | 2 | 3 | 4 |  1 | 2 | 3 | 4 |
5 | 6 | 7 | 8 |  5 | 6 | 7 | 8 |

A*B
1 + 10 + 3 + 20 | 2 + 12 + 6 + 24 | 3 + 14 + 9 + 28 | 4 + 16 + 12 + 32
5 + 30 + 7 + 40 |     

*/

/*
      /|
     / |
 a  /  |  c
   /   |
  ∠____|
    b


크기 행렬
x 0 0 0
0 y 0 0
0 0 z 0
0 0 0 1


                    Z축 회전 행렬
                    cos -sin    0   0
                    sin cos     0   0
                    0   0       1   0
                    0   0       0   1

Y축 회전 행렬            X축 회전 행렬 
cosY  0   sin 0         1   0   0   0
0    1   0   0          0  cos -sin 0
-sin 0   cos 0          0  sin  cos 0
0    0   0   1          0   0   0   1

Y * X
cosY | sinY * sinX | sinY * cosX | 0
 0   | cosX        | -sinX       | 0
-sinY| cosY * sinX | cosY * cosX | 0
 0   |  0          | 0           | 1

 [Y * X * Z]
 (cy * cz) + ( sy * sx * sz) | (cy * -sz) + (sy * sx * cz) | sy * cx | 0
 cx * sz                     | cx * cz                     | -sx     | 0
 (-sy * cz) + (cy * sx * sz) | (-sy * -sz) + (cy * sx * cz)| cy * cx | 0
 0                           | 0                           | 0       | 1

[월드행렬]
 x (cy * cz) + ( sy * sx * sz) | x(cy * -sz) + (sy * sx * cz) | x(sy * cx) | 0 ->각 행의 길이가 X의값
 y (cx * sz)                   | y (cx * cz)                  | y(-sx)     | 0 -> Y
 z(-sy * cz) + (cy * sx * sz)  | z(-sy * -sz) + (cy * sx * cz)| zcy * cx   | 0 -> Z
 tx                            |  ty                          | tz         | 1 --> tx : X축, tY : Y축, tZ : z축 이동

_13 : (sy * cx)
---------
_33 : (cy * cx)
--> tanY : 
atanY : Y축 회전

_23: -sinX :
asinX : X 축회전

_21 (cx * sz)
_22 (cx * cz)
-------------
-->tanZ 
atanZ : Z축 회전

Y * X * Z


이동 행렬
1   0   0   0
0   1   0   0
0   0   1   0
x   y   z   1

월드행렬
--> 크기행렬 * 자전(회전)행렬 * 이동행렬 * 부모행렬(나에게 영향을 주는 월드행렬)
   
*/