/*
    * MEJORAS POR IMPLEMENTAR: 
    - Fusion de los bucles, haciendo las operaciones para distintas fotografias a la vez, teniendo como topes los i y j de la imagen mas pequeña, j = 2600 e i = 1200
    - Unrolling de los bucles
*/

#include <stdio.h>
#include "pgmio.h"
#include <stdlib.h>
#include <sys/time.h>

void gradiente_morfologico(){
    //Matriz {Columnas, Filas}
    int dimA[] = {2600,1200};
    int dimB[] = {2600,2000};
    int dimC[] = {3200,2600};

    int cont = 0, i, j, ys, xs, limInferiorA = dimA[1] - 1, limInferiorB = dimB[1] - 1, limInferiorC = dimC[1] - 1, limDerechoA = dimA[0] - 1, limDerechoB = dimB[0] - 1, limDerechoC = dimC[0] - 1;
    unsigned char auxA = 0, auxB = 0, auxC = 0;

    //Imágenes iniciales
    unsigned char* imageA = loadPGM("Lab1_20_21_Images/A.pgm", &dimA[0], &dimA[1]);
    unsigned char* imageB = loadPGM("Lab1_20_21_Images/B.pgm", &dimB[0], &dimB[1]);
    unsigned char* imageC = loadPGM("Lab1_20_21_Images/C.pgm", &dimC[0], &dimC[1]);

    //Matrices de pixeles
    unsigned char (*matrixA)[dimA[0]] = malloc(sizeof(unsigned char[dimA[1]][dimA[0]]));
    unsigned char (*matrixB)[dimB[0]] = malloc(sizeof(unsigned char[dimB[1]][dimB[0]]));
    unsigned char (*matrixC)[dimC[0]] = malloc(sizeof(unsigned char[dimC[1]][dimC[0]]));

    //Matrices Dilatacion
    unsigned char (*matrixADilatacion)[dimA[0]] = malloc(sizeof(unsigned char[dimA[1]][dimA[0]]));
    unsigned char (*matrixBDilatacion)[dimB[0]] = malloc(sizeof(unsigned char[dimB[1]][dimB[0]]));
    unsigned char (*matrixCDilatacion)[dimC[0]] = malloc(sizeof(unsigned char[dimC[1]][dimC[0]]));

    //Matrices Erosión
    unsigned char (*matrixAErosion)[dimA[0]] = malloc(sizeof(unsigned char[dimA[1]][dimA[0]]));
    unsigned char (*matrixBErosion)[dimB[0]] = malloc(sizeof(unsigned char[dimB[1]][dimB[0]]));
    unsigned char (*matrixCErosion)[dimC[0]] = malloc(sizeof(unsigned char[dimC[1]][dimC[0]]));
    
    //Imagen final
    const char* filenameA= "imagenA.pgm";
    const char* filenameB= "imagenB.pgm";
    const char* filenameC= "imagenC.pgm";
    
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    
    for(i = 0; i < dimA[1]; i++){
        for(j = 0; j < dimA[0]; j++){
            matrixA[i][j] = imageA[cont];
            matrixB[i][j] = imageB[cont];
            matrixC[i][j] = imageC[cont + (i*600)];
            cont++;
        }
    }
    //Incompletas de B y C
    for(i = dimA[1]; i < dimB[1]; i++){
        for(j = 0; j < dimB[0]; j++){
            matrixB[i][j] = imageB[cont];
            matrixC[i][j] = imageC[cont + (i*600)];
            cont++;
        }
    }
    //Incompletas de C
    
    for(i = 0; i < dimB[1]; i++){
        for(j = dimB[0]; j < dimC[0]; j++){
            cont = i*dimC[0] + j;
            matrixC[i][j] = imageC[cont];
        }
    }
    cont = dimB[1] * dimC[0];

    for(i = dimB[1]; i < dimC[1]; i++){
        for(j = 0; j < dimC[0]; j++){
            matrixC[i][j] = imageC[cont];
            cont++;
        }
    }

    //DILATACION = pixel toma el valor mas alto dentro de su ventana 3x3
    //Hacer un recorrido independiente por las que son diferentes y no hay que analizar las 9 posiciones 
    /*
        a  b  c
        d [e] f
        g  h  i 
    */
    
    
    for(i = 0; i < dimA[1]; i++){//Principal: A
        for(j = 0; j < dimA[0]; j++){
            if(i == 0){//Arriba
                if(j == 0){//Arriba izquierda * A, B, C *
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC < matrixB[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    matrixBDilatacion[i][j] = auxB;
                    matrixCDilatacion[i][j] = auxC;
                    auxA = 0;
                    auxB = 0;
                    auxC = 0;
                }
                else if(j == limDerechoA){//Arriba derecha * A, B * 
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                               
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    matrixBDilatacion[i][j] = auxB;
                    auxA = 0;
                    auxB = 0;
                }
                else{//Arriba medio * A, "B", "C" * 
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    matrixBDilatacion[i][j] = auxB;
                    matrixCDilatacion[i][j] = auxC;
                    auxA = 0;
                    auxB = 0;
                    auxC = 0;
                }
            }
            else if(i == limInferiorA){//Abajo
                if(j == 0){//Abajo izquierda * A *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    auxA = 0;
                }
                else if(j == limDerechoA){//Abajo derecha * A *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    auxA = 0;
                }
                else{//Abajo medio * A *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    auxA = 0;
                }
            }
            else{//Medio
                if(j == 0){//Medio izquierda * A, "B", "C" * 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    matrixBDilatacion[i][j] = auxB;
                    matrixCDilatacion[i][j] = auxC;
                    auxA = 0;
                    auxB = 0;
                    auxC = 0;
                }
                else if(j == limDerechoA){//Medio derecha * A , "B" *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB < matrixA[ys][xs])
                               auxB = matrixA[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    matrixBDilatacion[i][j] = auxB;
                    auxA = 0;
                    auxB = 0;
                }
                else{//Medio total * A, "B", "C" *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxA < matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixADilatacion[i][j] = auxA;
                    matrixBDilatacion[i][j] = auxB;
                    matrixCDilatacion[i][j] = auxC;
                    auxA = 0;
                    auxB = 0;
                    auxC = 0;
                }
            }
        }
    }

    for(i = limInferiorA; i < dimB[1]; i++){//Principal: B
        for(j = 0; j < dimB[0]; j++){
            if((i == limInferiorB)){//Abajo
                if(j == 0){//Abajo izquierda * A, B *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBDilatacion[i][j] = auxB;
                    auxB = 0;
                }
                else if(j == limDerechoB){//Abajo derecha * A, B *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBDilatacion[i][j] = auxB;
                    auxB = 0;
                }
                else{//Abajo medio * A, B *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBDilatacion[i][j] = auxB;
                    auxB = 0;
                }
            }
            else{//Medio
                if(j == 0){//Medio izquierda * A, B, "C" * 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixBDilatacion[i][j] = auxB;
                    matrixCDilatacion[i][j] = auxC;
                    auxB = 0;
                    auxC = 0;
                }
                else if(j == limDerechoA){//Medio derecha * A, B *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBDilatacion[i][j] = auxB;
                    auxB = 0;
                }
                else{//Medio total * A, B, "C" *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxB < matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixBDilatacion[i][j] = auxB;
                    matrixCDilatacion[i][j] = auxC;
                    auxB = 0;
                    auxC = 0;
                }
            }
        }
    }

    for(i = 0; i < dimB[1]; i++){//Principal: C (Parte superior derecha)
        for(j = limDerechoB; j < dimC[0]; j++){
            if(i == 0){//Arriba
                if(j == limDerechoC){//Arriba derecha
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
                else{//Arriba medio
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
            }
            
            else{//Medio
                if(j == limDerechoC){//Medio derecha
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
                else{//Medio total 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
            }
        }
    }

    for(i = limInferiorB; i < dimC[1]; i++){//Principal: C (Parte inferior)
        for(j = 0; j < dimC[0]; j++){
            if((i == limInferiorC)){//Abajo
                if(j == 0){//Abajo izquierda
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
                else if(j == limDerechoC){//Abajo derecha
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
                else{//Abajo medio 
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
            }

            else{//Medio
                if(j == 0){//Medio izquierda
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
                else if(j == limDerechoC){//Medio derecha
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
                else{//Medio total 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC < matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCDilatacion[i][j] = auxC;
                    auxC = 0;
                }
            }
        }
    }
    
    
    //EROSION  = pixel toma el valor mas bajo dentro de su ventana 3x3
    auxA = 255, auxB = 255, auxC = 255;
    
    for(i = 0; i < dimA[1]; i++){//Principal: A
        for(j = 0; j < dimA[0]; j++){
            if(i == 0){//Arriba
                if(j == 0){//Arriba izquierda * A, B, C *
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC > matrixB[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    matrixBErosion[i][j] = auxB;
                    matrixCErosion[i][j] = auxC;
                    auxA = 255;
                    auxB = 255;
                    auxC = 255;
                }
                else if(j == limDerechoA){//Arriba derecha * A, B * 
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                               
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    matrixBErosion[i][j] = auxB;
                    auxA = 255;
                    auxB = 255;
                }
                else{//Arriba medio * A, "B", "C" * 
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    matrixBErosion[i][j] = auxB;
                    matrixCErosion[i][j] = auxC;
                    auxA = 255;
                    auxB = 255;
                    auxC = 255;
                }
            }
            else if((i == limInferiorA)){//Abajo
                if(j == 0){//Abajo izquierda * A *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    auxA = 255;
                }
                else if(j == limDerechoA){//Abajo derecha * A *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    auxA = 255;
                }
                else{//Abajo medio * A *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    auxA = 255;
                }
            }
            else{//Medio
                if(j == 0){//Medio izquierda * A, "B", "C" * 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    matrixBErosion[i][j] = auxB;
                    matrixCErosion[i][j] = auxC;
                    auxA = 255;
                    auxB = 255;
                    auxC = 255;
                }
                else if(j == limDerechoA){//Medio derecha * A , "B" *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB > matrixA[ys][xs])
                               auxB = matrixA[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    matrixBErosion[i][j] = auxB;
                    auxA = 255;
                    auxB = 255;
                }
                else{//Medio total * A, "B", "C" *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxA > matrixA[ys][xs])
                               auxA = matrixA[ys][xs]; 
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixAErosion[i][j] = auxA;
                    matrixBErosion[i][j] = auxB;
                    matrixCErosion[i][j] = auxC;
                    auxA = 255;
                    auxB = 255;
                    auxC = 255;
                }
            }
        }
    }

    for(i = limInferiorA; i < dimB[1]; i++){//Principal: B
        for(j = 0; j < dimB[0]; j++){
            if((i == limInferiorB)){//Abajo
                if(j == 0){//Abajo izquierda * A, B *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBErosion[i][j] = auxB;
                    auxB = 255;
                }
                else if(j == limDerechoB){//Abajo derecha * A, B *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBErosion[i][j] = auxB;
                    auxB = 255;
                }
                else{//Abajo medio * A, B *
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBErosion[i][j] = auxB;
                    auxB = 255;
                }
            }
            else{//Medio
                if(j == 0){//Medio izquierda * A, B, "C" * 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixBErosion[i][j] = auxB;
                    matrixCErosion[i][j] = auxC;
                    auxB = 255;
                    auxC = 255;
                }
                else if(j == limDerechoA){//Medio derecha * A, B *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                        }
                    }
                    matrixBErosion[i][j] = auxB;
                    auxB = 255;
                }
                else{//Medio total * A, B, "C" *
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxB > matrixB[ys][xs])
                               auxB = matrixB[ys][xs]; 
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixBErosion[i][j] = auxB;
                    matrixCErosion[i][j] = auxC;
                    auxB = 255;
                    auxC = 255;
                }
            }
        }
    }

    for(i = 0; i < dimB[1]; i++){//Principal: C (Parte superior derecha)
        for(j = limDerechoB; j < dimC[0]; j++){
            if(i == 0){//Arriba
                if(j == limDerechoC){//Arriba derecha
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
                else{//Arriba medio
                    for(ys = i; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
            }
            
            else{//Medio
                if(j == limDerechoC){//Medio derecha
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
                else{//Medio total 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
            }
        }
    }

    for(i = limInferiorB; i < dimC[1]; i++){//Principal: C (Parte inferior)
        for(j = 0; j < dimC[0]; j++){
            if((i == limInferiorC)){//Abajo
                if(j == 0){//Abajo izquierda
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
                else if(j == limDerechoC){//Abajo derecha
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
                else{//Abajo medio 
                    for(ys = i-1; ys <= i; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
            }

            else{//Medio
                if(j == 0){//Medio izquierda
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j; xs <= j+1; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
                else if(j == limDerechoC){//Medio derecha
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
                else{//Medio total 
                    for(ys = i-1; ys <= i+1; ys++){
                        for(xs = j-1; xs <= j+1; xs++){
                            if(auxC > matrixC[ys][xs])
                               auxC = matrixC[ys][xs]; 
                        }
                    }
                    matrixCErosion[i][j] = auxC;
                    auxC = 255;
                }
            }
        }
    }

    //GRADIENTE MORFOLOGICO = DILATACION - EROSION
    cont = 0;
    //A entero
    for(i = 0; i < dimA[1]; i++){
        for(j = 0; j < dimA[0]; j++){
            imageA[cont] = matrixADilatacion[i][j] - matrixAErosion[i][j];
            imageB[cont] = matrixBDilatacion[i][j] - matrixBErosion[i][j];
            imageC[cont + (i*600)] = matrixCDilatacion[i][j] - matrixCErosion[i][j];
            cont++;
        }
    }

    //Incompletas de B y C
    for(i = dimA[1]; i < dimB[1]; i++){
        for(j = 0; j < dimB[0]; j++){
            imageB[cont] = matrixBDilatacion[i][j] - matrixBErosion[i][j];
            imageC[cont + (i*600)] = matrixCDilatacion[i][j] - matrixCErosion[i][j];
            cont++;
        }
    }

    //Incompletas de C
    for(i = 0; i < dimB[1]; i++){
        for(j = dimB[0]; j < dimC[0]; j++){
            cont = i*dimC[0] + j;
            imageC[cont] = matrixCDilatacion[i][j] - matrixCErosion[i][j];
        }
    }
    cont = dimB[1] * dimC[0];
    for(i = dimB[1]; i < dimC[1]; i++){
        for(j = 0; j < dimC[0]; j++){
            imageC[cont] = matrixCDilatacion[i][j] - matrixCErosion[i][j];
            cont++;
        }
    }    

    gettimeofday(&stop, NULL);
    printf("%f\n", (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec));
    //Imprimir resultados
    exportPGM(filenameA, imageA, dimA[0], dimA[1]);
    exportPGM(filenameB, imageB, dimB[0], dimB[1]);
    exportPGM(filenameC, imageC, dimC[0], dimC[1]);

    //Liberar memoria
    free(imageA);
    free(imageB);
    free(imageC);
    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(matrixADilatacion);
    free(matrixBDilatacion);
    free(matrixCDilatacion);
    free(matrixAErosion);
    free(matrixBErosion);
    free(matrixCErosion);
}

int main(){
    //struct timeval stop, start;
    //gettimeofday(&start, NULL);
    gradiente_morfologico();
    //gettimeofday(&stop, NULL);
    //printf("%lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    return 0;
}
