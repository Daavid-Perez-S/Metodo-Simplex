// Programa hecho por: David Pérez Sánchez  3ª "A"
// UP-Chiapas  I.D.S.
// Completado el 20 de Junio de 2017, en 4 días.

#include <iostream>
#include <stdio.h>
#include <conio2.h>	// Esta librería es una actualización de conio.h , necesita estar instalada en el equipo para que el programa funcione
#include <clocale>	// Librería necesaria para la conf. del idioma
#include <vector>
#include <windows.h>
#include <iomanip>
#include <string>
using namespace std;

// Prototipos de Funciones

// LLenado de Datos
void llenadoDeLaVariableZ(vector <double> &, double, bool *);
void llenadoDeLaVariableX(vector <double> &, double, int, bool*);
void llenadoDeLaVariableS(vector <double> &, double,int, bool*);
void llenadoDelLadoDerechoDeLaEcuacion(vector <double> &, double, bool *);
void llenadoDeMatrizSimplex(vector < vector <double> > &, vector <double> &);
void llenarFilaDeVariablesLetras(vector <string> &, int, int);
void rellenarColumnaDeVariablesLetras(vector <string> &, vector <string> &, int, int);
void llenarColumnaDeVariablesLetras(vector <string> &, int);
void transferirNuevaFilaPivote(vector < vector <double> > &, vector <double> &, int, int);

// Obtenciones de datos
int encontrarCriterioDeOptimalidadSimplex(vector < vector <double> > &, int);
int encontrarCriterioDeFactibilidadSimplex(vector < vector <double> > &, vector <double> &, vector <double> &);
void obtenerColumnaSolucion(vector < vector <double> > &, vector <double> &, int, int);
void obtenerColumnaPivote(vector < vector <double> > &, vector <double> &, int, int);
double obtenerElementoPivote(vector < vector <double> > &, int, int);
void obtenerNuevaFilaPivote(vector < vector <double> > &, vector <double> &, int, int, double);
void obtenerNuevasFilasNormales(vector < vector <double> > &, vector <double> &, vector <double> &, int, int, int);
void obtenerNumerosEnFilaZ(vector < vector <double> > &, vector <double> &, int);
bool comprobarExistenciaDeNumerosNegativosEnLaFilaZ(vector <double> &);

// Vaciados, impresiones y comprobaciones
void vaciarMatrizSimplex(vector < vector <double> > &matrizSimplex, int, int);
void imprimirArregloDeInsercionDeValores(vector <double> &);
void imprimirColumnaDePalabras(vector <string> &);
void imprimirArregloDePalabras(vector <string> &);
void imprimirMatrizSimplex(vector < vector <double> > &, vector <string> &, vector <string> &, int, int);
bool comprobacionDeNumeroMenorACero(double);
bool comprobacionDeNumeroMenorOIgualACero(double);
double convertirNumeroANegativo(double);
	
int main(){
	
	setlocale(LC_ALL,"spanish");	// Configura el programa en Español México
	
	int numeroDeVariables;
	int numeroDeRestricciones;
	int numeroDeColumnas;
	int numeroDeFilas;
	int indiceDeCriterioDeOptimalidadSimplex;
	int indiceDeCriterioDeFactibilidadSimplex;
	double valorDeElementoPivote;
	double valorUsuario;
	bool numeroNegativo;
	
	int contadorDeIteraciones= 1;
	bool banderaDeLlenadoDePrimeraVez= true;	// Sirve solo en el llenado de la fila Z
	bool existenciaDeNumerosNegativosEnFilaZ= true;	// Sirve para seguir iterando en el ciclo do-while
	
	clrscr();
	// Inserción del número de variables y restricciones que el usuario va a utlizar
	system("title  MÉTODO SIMPLEX");
	cout<<" MÉTODO SIMPLEX"<< endl;
	cout<<"-----------------------"<< endl<< endl;
	cout<<" Ingrese el número de variables a utilizar: ";
	cin>> numeroDeVariables;
	cout<<" Ingrese el número de restricciones a utilizar: ";
	cin>> numeroDeRestricciones;
	
	// Creación de la matriz simplex y otros vectores
	numeroDeColumnas= numeroDeVariables + numeroDeRestricciones + 2;
	numeroDeFilas= numeroDeRestricciones + 1;
	vector <double> arregloDeInsercionDeValores(numeroDeColumnas);
	vector < vector <double> > matrizSimplex(numeroDeFilas, arregloDeInsercionDeValores);
	vector <double> columnaPivote;
	vector <double> columnaSolucion;
	vector <double> nuevaFilaPivote;
	vector <double> numerosDeFilaZ;
	vector <string> filaDeVariablesLetras;
	vector <string> columnaDeVariablesLetras;
	
	llenarFilaDeVariablesLetras(filaDeVariablesLetras, numeroDeVariables, numeroDeRestricciones);
	llenarColumnaDeVariablesLetras(columnaDeVariablesLetras, numeroDeRestricciones);
	
	// Llenado de la Matriz simplex con datos del usuario
	for(int i=0; i<numeroDeFilas; i++){
		clrscr();
		arregloDeInsercionDeValores.clear();
		arregloDeInsercionDeValores.begin();
		if(i==0){
			cout<<" MAXIMIZAR Z"<< endl;
			cout<<"-----------------------"<< endl<< endl;
		}else{
			cout<<" RESTRICCIÓN "<< i<< endl;
			cout<<"-----------------------"<< endl<< endl;
		}
		
		llenadoDeLaVariableZ(arregloDeInsercionDeValores, valorUsuario, &banderaDeLlenadoDePrimeraVez);
		llenadoDeLaVariableX(arregloDeInsercionDeValores, valorUsuario, numeroDeVariables, &banderaDeLlenadoDePrimeraVez);
		llenadoDeLaVariableS(arregloDeInsercionDeValores, valorUsuario, numeroDeRestricciones, &banderaDeLlenadoDePrimeraVez);
		llenadoDelLadoDerechoDeLaEcuacion(arregloDeInsercionDeValores, valorUsuario, &banderaDeLlenadoDePrimeraVez);
		imprimirArregloDeInsercionDeValores(arregloDeInsercionDeValores);
		llenadoDeMatrizSimplex(matrizSimplex, arregloDeInsercionDeValores);
	}
	clrscr();
	cout<<" Se imprime matriz llenada:"<< endl<< endl;
	imprimirMatrizSimplex(matrizSimplex, filaDeVariablesLetras, columnaDeVariablesLetras,numeroDeFilas, numeroDeColumnas);	
	// Bloque de Algoritmo de Operaciones para resolver la Matriz Simplex
	do{
		indiceDeCriterioDeOptimalidadSimplex= encontrarCriterioDeOptimalidadSimplex(matrizSimplex, numeroDeColumnas);
		cout<<"\n\n\n Indice de Criterio de Optimalidad: "<< indiceDeCriterioDeOptimalidadSimplex<< endl;
		obtenerColumnaPivote(matrizSimplex, columnaPivote, indiceDeCriterioDeOptimalidadSimplex, numeroDeRestricciones);
		obtenerColumnaSolucion(matrizSimplex, columnaSolucion, numeroDeRestricciones, numeroDeColumnas);
		
		indiceDeCriterioDeFactibilidadSimplex= encontrarCriterioDeFactibilidadSimplex(matrizSimplex, columnaSolucion, columnaPivote);
		if(indiceDeCriterioDeFactibilidadSimplex > 0){
			
			cout<<" Indice de Criterio de Factibilidad: "<< indiceDeCriterioDeFactibilidadSimplex<< endl<< endl;
			valorDeElementoPivote= obtenerElementoPivote(matrizSimplex, indiceDeCriterioDeOptimalidadSimplex, indiceDeCriterioDeFactibilidadSimplex);
			cout<<" El valor del elemento pivote es de: "<< valorDeElementoPivote<< endl<< endl;
			obtenerNuevaFilaPivote(matrizSimplex, nuevaFilaPivote, indiceDeCriterioDeFactibilidadSimplex, numeroDeColumnas, valorDeElementoPivote);
			transferirNuevaFilaPivote(matrizSimplex, nuevaFilaPivote, indiceDeCriterioDeFactibilidadSimplex, numeroDeColumnas);
			obtenerNuevasFilasNormales(matrizSimplex, columnaPivote, nuevaFilaPivote, numeroDeFilas, numeroDeColumnas, indiceDeCriterioDeFactibilidadSimplex);
			
			cout<<"\n ------------------------------------------------------"<< endl<< endl;
			cout<<" "<< contadorDeIteraciones<<"º Iteración"<< endl<< endl;
			rellenarColumnaDeVariablesLetras(filaDeVariablesLetras, columnaDeVariablesLetras, indiceDeCriterioDeOptimalidadSimplex, indiceDeCriterioDeFactibilidadSimplex);
			imprimirMatrizSimplex(matrizSimplex, filaDeVariablesLetras, columnaDeVariablesLetras,numeroDeFilas, numeroDeColumnas);
			//cout<<"\n ------------------------------------------------------"<< endl<< endl;
			
			obtenerNumerosEnFilaZ(matrizSimplex, numerosDeFilaZ, numeroDeColumnas);
			existenciaDeNumerosNegativosEnFilaZ= comprobarExistenciaDeNumerosNegativosEnLaFilaZ(numerosDeFilaZ);
			
			contadorDeIteraciones++;
		}else{
			// En caso de que el indice de criterio de factibilidad simplez fuera menor a 0, significa que hubo un error y no pudo completar la opración
			// Osea que la columna de valores divididos no tiene ningún valor. Por lo tanto, la matriz no se puede resolver
			MessageBox(NULL,"No se encontraron valores válidos, se abortó la operación","Error", MB_OK | MB_ICONWARNING | MB_DEFBUTTON1);
			break;
		}
	}while(existenciaDeNumerosNegativosEnFilaZ == true);
	if(indiceDeCriterioDeFactibilidadSimplex > 0)
		MessageBox(NULL,"Método Simplex completado exitosamente","Matriz Simplex", MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1);
	
	getch();
	return 0;
}

void llenadoDeLaVariableZ(vector <double> &arregloDeInsercionDeValores, double valorUsuario,bool *banderaDeLlenadoDePrimeraVez){
	
	bool numeroNegativo;
	arregloDeInsercionDeValores.begin();
	
	if(*banderaDeLlenadoDePrimeraVez == true)
		arregloDeInsercionDeValores.push_back(1);
	else{
		arregloDeInsercionDeValores.push_back(0);
	}
	cout<<"\n";
}

void llenadoDeLaVariableX(vector <double> &arregloDeInsercionDeValores, double valorUsuario, int numeroDeVariables, bool *banderaDeLlenadoDePrimeraVez){
	
	bool numeroNegativo;
	
	for(int i=0; i<numeroDeVariables; i++){
		do{
			if(*banderaDeLlenadoDePrimeraVez == true)
				cout<<" Ingrese X"<<i+1<<": ";
			else
				cout<<" Ingrese X"<<i+1<<" si existe, 0 en caso contrario: ";
			cin>> valorUsuario;
			if(*banderaDeLlenadoDePrimeraVez == true){
				numeroNegativo= comprobacionDeNumeroMenorOIgualACero(valorUsuario);
				if(numeroNegativo == false){
					valorUsuario= convertirNumeroANegativo(valorUsuario);
					arregloDeInsercionDeValores.push_back(valorUsuario);
				}else
					MessageBox(NULL,"No se aceptan valores negativos, intente de nuevo","Error", MB_OK | MB_ICONWARNING | MB_DEFBUTTON1);
			}else{
				arregloDeInsercionDeValores.push_back(valorUsuario);
			}
		}while(numeroNegativo == true);
	}
	cout<<"\n";
}

void llenadoDeLaVariableS(vector <double> &arregloDeInsercionDeValores, double valorUsuario, int numeroDeRestricciones, bool *banderaDeLlenadoDePrimeraVez){
	
	bool numeroNegativo;
	static int indiceDeLlenadoDeS= 0; // Nos ayuda a saber en que lugar debemos poner 0 y donde un 1 automáticamente
	
	if(*banderaDeLlenadoDePrimeraVez == true)
		for(int i=0; i<numeroDeRestricciones; i++)
			arregloDeInsercionDeValores.push_back(0);
	else{
		for(int i=0; i<numeroDeRestricciones; i++){
			if(i == indiceDeLlenadoDeS)
				arregloDeInsercionDeValores.push_back(1);
			else
				arregloDeInsercionDeValores.push_back(0);
		}
		indiceDeLlenadoDeS++;
	}
	cout<<"\n";
}

void llenadoDelLadoDerechoDeLaEcuacion(vector <double> &arregloDeInsercionDeValores, double valorUsuario, bool *banderaDeLlenadoDePrimeraVez){
	
	bool numeroNegativo;
	if(*banderaDeLlenadoDePrimeraVez == true)
		arregloDeInsercionDeValores.push_back(0);
	else{
		do{
			cout<<" Ingrese el valor derecho de la ecuación: ";
			cin>> valorUsuario;
			numeroNegativo= comprobacionDeNumeroMenorACero(valorUsuario);
			if(numeroNegativo == false)
				arregloDeInsercionDeValores.push_back(valorUsuario);
			else
				MessageBox(NULL,"No se aceptan valores negativos, intente de nuevo","Error", MB_OK | MB_ICONWARNING | MB_DEFBUTTON1);
		}while(numeroNegativo == true);
	}
	*banderaDeLlenadoDePrimeraVez= false;
}

void llenadoDeMatrizSimplex(vector < vector <double> > &matrizSimplex, vector <double> &arregloDeInsercionDeValores){
	
	static int posicionFila= 0; // Nos ayuda a saber en que fila quedamos anteriormente para llenar esa fila que se nos indica
	
	for(int j=0; j<arregloDeInsercionDeValores.size(); j++){
		matrizSimplex[posicionFila][j]= arregloDeInsercionDeValores[j];
	}
	posicionFila++;
}

void llenarFilaDeVariablesLetras(vector <string> &filaDeVariablesLetras, int numeroDeVariables, int numeroDeRestricciones){
	
	string palabra;
	int aux;
	
	filaDeVariablesLetras.push_back("Básicas");
	filaDeVariablesLetras.push_back("Z");
	for(int i=0; i<numeroDeVariables; i++){
		aux= i+1;
		palabra= "X"+to_string(aux);
		filaDeVariablesLetras.push_back(palabra);
	}
	for(int k=0; k<numeroDeRestricciones; k++){
		aux= k+1;
		palabra= "S"+to_string(aux);
		filaDeVariablesLetras.push_back(palabra);
	}
	filaDeVariablesLetras.push_back("Solución");
	
}

void rellenarColumnaDeVariablesLetras(vector <string> &filaDeVariablesLetras, vector <string> &columnaDeVariablesLetras, int indiceDeCriterioDeOptimalidadSimplex, int indiceDeCriterioDeFactibilidadSimplex){
	
	string palabra;
	
	palabra= filaDeVariablesLetras[indiceDeCriterioDeOptimalidadSimplex+1];
	columnaDeVariablesLetras[indiceDeCriterioDeFactibilidadSimplex]= palabra;
	
}

void llenarColumnaDeVariablesLetras(vector <string> &columnaDeVariablesLetras, int numeroDeRestricciones){
	
	string palabra;
	int aux;
	
	columnaDeVariablesLetras.push_back("Z");
	for(int k=0; k<numeroDeRestricciones; k++){
		aux= k+1;
		palabra= "S"+to_string(aux);
		columnaDeVariablesLetras.push_back(palabra);
	}
	
}

void transferirNuevaFilaPivote(vector < vector <double> > &matrizSimplex, vector <double> &nuevaFilaPivote, int indiceDeCriterioDeFactibilidadSimplex, int numeroDeColumnas){
	
	// Este bloque de código solo copia la nueva fila pivote que se obtuvo a la matriz principal
	for(int j=0; j<numeroDeColumnas; j++){
		matrizSimplex[indiceDeCriterioDeFactibilidadSimplex][j]= nuevaFilaPivote[j];
	}
}

int encontrarCriterioDeOptimalidadSimplex(vector < vector <double> > &matrizSimplex, int numeroDeColumnas){
	
	// El criterio de optimalidad consiste en encontrar el numero mayor negativo de la fila Z únicamente
	
	int indiceDeValorMasNegativo=0, j;
	double valorMasNegativo;
	
	valorMasNegativo= matrizSimplex[0][0];
	for(j=0; j<(numeroDeColumnas-1); j++){
		if(matrizSimplex[0][j] < valorMasNegativo){
			valorMasNegativo= matrizSimplex[0][j];
			indiceDeValorMasNegativo= j;
		}
	}
	return indiceDeValorMasNegativo;
}

int encontrarCriterioDeFactibilidadSimplex(vector < vector <double> > &matrizSimplex, vector <double> &columnaSolucion, vector <double> &columnaPivote){
		
	int indiceDeMenorPositivo, indiceDeError= -1, contadorDeDesplazamiento=0;
	double valorAuxiliar, valorPasar;
	bool bandera= false;
	vector <double> columnaDeValoresDivididos;
	vector <double> columnaDeIndicesDeValoresDivididos;
	vector <double> columnaPivoteAuxiliar;
	
	// Se copia los valores a una columna pivote auxiliar, exeptuando al índice 1
	for(int k=1; k<columnaPivote.size(); k++){
		valorPasar= columnaPivote[k];
		columnaPivoteAuxiliar.push_back(valorPasar);
	}
	
	// Si el valor cumple con el requisito, se hace la opración y se almacena el resultado y su índice
	for(int i=0; i<columnaPivoteAuxiliar.size(); i++){
		if(columnaPivoteAuxiliar[i] > 0 ){
			valorAuxiliar= columnaSolucion[i] / columnaPivoteAuxiliar[i];
			columnaDeValoresDivididos.push_back(valorAuxiliar);
			columnaDeIndicesDeValoresDivididos.push_back(i);
		}
		if(columnaDeIndicesDeValoresDivididos.size() == 0){
			bandera= true;
			contadorDeDesplazamiento++; // 2
		}
		else
			bandera= false;
	}
	
	// Tenemos que encontrar el número menor de todos los que resultaron exitosamente divididos
	
	if(columnaDeValoresDivididos.size() > 0){
		valorAuxiliar= columnaDeValoresDivididos[0];
		indiceDeMenorPositivo= columnaDeIndicesDeValoresDivididos[0];
		for(int j=0; j<columnaDeValoresDivididos.size(); j++){
			if(columnaDeValoresDivididos[j] < valorAuxiliar){
				valorAuxiliar= columnaDeValoresDivididos[j];
				indiceDeMenorPositivo= columnaDeIndicesDeValoresDivididos[j];
			}
		}
		if(bandera == true)
			indiceDeMenorPositivo= indiceDeMenorPositivo + contadorDeDesplazamiento;
		return indiceDeMenorPositivo + 1;
	}else
		return indiceDeError;
}

void obtenerColumnaSolucion(vector < vector <double> > &matrizSimplex, vector <double> &columnaSolucion, int numeroDeRestricciones, int numeroDeColumnas){
	
	// Este bloque de código sólo se encarga de obtener la última columna de la matriz, excepto el coheficiente de la fila Z
	
	columnaSolucion.clear();
	for(int i=1; i<=numeroDeRestricciones; i++){
		columnaSolucion.push_back(matrizSimplex[i][numeroDeColumnas-1]);
	}
}

void obtenerColumnaPivote(vector < vector <double> > &matrizSimplex, vector <double> &columnaPivote, int indiceDeCriterioDeOptimalidadSimplex, int numeroDeRestricciones){
	
	// Este bloque de código se encarga de obtener toda la columna pivote
	
	columnaPivote.clear();
	for(int i=0; i<=numeroDeRestricciones; i++){
		columnaPivote.push_back(matrizSimplex[i][indiceDeCriterioDeOptimalidadSimplex]);
	}
}

double obtenerElementoPivote(vector < vector <double> > &matrizSimplex, int indiceDeCriterioDeOptimalidadSimplex, int indiceDeCriterioDeFactibilidadSimplex){
	
	// Ya con las coordenadas en I y J, podemos obtener el elemento pivote que se encuentra en esa ubicación
	
	double valorDeElementoPivote;
	valorDeElementoPivote= matrizSimplex[indiceDeCriterioDeFactibilidadSimplex][indiceDeCriterioDeOptimalidadSimplex];
	return valorDeElementoPivote;
}

void obtenerNuevaFilaPivote(vector < vector <double> > &matrizSimplex, vector <double> &nuevaFilaPivote, int indiceDeCriterioDeFactibilidadSimplex, int numeroDeColumnas, double valorDeElementoPivote){
	
	// Aqui tomamos la fila pivote y obtenemos la nueva fila pivote, después de haberla dividio entre el elemento pivote obtenido anteiormente
	// NUEVA_FILA_PIVOTE= FILA_PIVOTE_ACTUAL / ELEMENTO_PIVOTE
	
	nuevaFilaPivote.clear();
	vector <double> arregloAuxiliar;
	double valorAuxiliar;
	
	for(int j=0; j<numeroDeColumnas; j++){
		valorAuxiliar= matrizSimplex[indiceDeCriterioDeFactibilidadSimplex][j];
		arregloAuxiliar.push_back(valorAuxiliar);
		valorAuxiliar= arregloAuxiliar[j] / valorDeElementoPivote;
		nuevaFilaPivote.push_back(valorAuxiliar);
	}
}

void obtenerNuevasFilasNormales(vector < vector <double> > &matrizSimplex, vector <double> &columnaPivote, vector <double> &nuevaFilaPivote, int numeroDeFilas, int numeroDeColumnas, int indiceDeCriterioDeFactibilidadSimplex){
	
	// Aquí vamos obteniendo las nuevas filas restantes mediante la ecuación:
	// NUEVA FILA NORMAL = FILA ACTUAL - ( COHEFICIENTE ACTUAL DE LA FILA PIVOTE * FILA PIVOTE )
	
	double valorAuxiliar;
	
	for(int i=0; i<numeroDeFilas; i++){
		if(i != indiceDeCriterioDeFactibilidadSimplex){
			for(int j=0; j<numeroDeColumnas; j++){
				valorAuxiliar= matrizSimplex[i][j] - (columnaPivote[i] * nuevaFilaPivote[j]);
				matrizSimplex[i][j]= valorAuxiliar;
			}
		}
	}
}

void obtenerNumerosEnFilaZ(vector < vector <double> > &matrizSimplex, vector <double> &numerosDeFilaZ, int numeroDeColumnas){
	
	// Aquí lo único que hacemos es obtener todos los valores de la nueva fila Z
	
	numerosDeFilaZ.clear();
	double valorAuxiliar;
	
	for(int j=0; j<numeroDeColumnas; j++){
		valorAuxiliar= matrizSimplex[0][j];
		numerosDeFilaZ.push_back(valorAuxiliar);
	}
}

bool comprobarExistenciaDeNumerosNegativosEnLaFilaZ(vector <double> &numerosDeFilaZ){
	
	// Comprobamos que en la fila Z, al final de la iteración, aún existan valores negativos para seguir iterando en el bucle do-while
		
	bool existenciaDeNumerosNegativosEnFilaZ;
	
	for(int i=0; i<numerosDeFilaZ.size(); i++){
		if(numerosDeFilaZ[i] >= 0)
			existenciaDeNumerosNegativosEnFilaZ= false;
		else{
			existenciaDeNumerosNegativosEnFilaZ= true;	
			return existenciaDeNumerosNegativosEnFilaZ;
		}
	}
	return existenciaDeNumerosNegativosEnFilaZ;
}

void vaciarMatrizSimplex(vector < vector <double> > &matrizSimplex, int numeroDeFilas, int numeroDeColumnas){
	
	// Aquí solo vaciamos la matriz principal, dejamos todos los espacios con valor 0
	
	for(int i=0; i<numeroDeFilas; i++){
		for(int j=0; j<numeroDeColumnas; j++){
			matrizSimplex[i][j]= 0;
		}
	}
}

void imprimirArregloDeInsercionDeValores(vector <double> &arregloDeInsercionDeValores){
	
	// Este bloque de código sólamente imprime el buffer de entrada, o cualquier vector unidimensional de valores
	
	cout<<"\n\n";
	for(int i=0; i<arregloDeInsercionDeValores.size(); i++)
		cout<< setw(5)<< left<< " "<< setw(5)<< arregloDeInsercionDeValores[i];
	cout<<"\n\n";
}

void imprimirArregloDePalabras(vector <string> &arregloDePalabras){
	
	// Este bloque de código imprime cualquier vector unidimensional de palabras en forma horizontal
	
	cout<<"\n\n";
	for(int i=0; i<arregloDePalabras.size(); i++)
		cout<< setw(6)<< left<< " "<< setw(6)<< arregloDePalabras[i];
	cout<<"\n\n";
}

void imprimirColumnaDePalabras(vector <string> &arregloDePalabras){
	
	// Este bloque de código imprime cualquier vector unidimensional de palabras en forma vertical
	static int i= 0;
	
	cout<< setw(6)<< left<< " "<< setw(6)<< arregloDePalabras[i];
	i++;
	if(i == arregloDePalabras.size() ){
		i= 0;
	}
}

void imprimirMatrizSimplex(vector < vector <double> > &matrizSimplex, vector <string> &filaDePalabras, vector <string> &columnaDePalabras,int numeroDeFilas, int numeroDeColumnas){
	
	// Con esto imprimimos todos los valores que contenga la matriz simplex
	
	printf("\n\n");
	imprimirArregloDePalabras(filaDePalabras);

		for(int i=0; i<numeroDeFilas; i++){
			imprimirColumnaDePalabras(columnaDePalabras);
			for(int j=0; j<numeroDeColumnas; j++)
				cout<< setw(6)<< left<< " "<< setw(6)<< setprecision(2) << fixed << matrizSimplex[i][j];
			//printf("\t%.2f", matrizSimplex[i][j]);
			cout<<"\n";
		}
}

bool comprobacionDeNumeroMenorACero(double numeroIntroducidoPorUsuario){
	
	// La primera vez que inserta el usuario los valores de la fila Z, se comprueba que algunos casos no sean negativos
	
	bool numeroNegativo= false;
	if(numeroIntroducidoPorUsuario <0)
		numeroNegativo= true;
	return numeroNegativo;
}

bool comprobacionDeNumeroMenorOIgualACero(double numeroIntroducidoPorUsuario){
	
	// La primera vez que inserta el usuario los valores de la fila Z, se comprueba que algunos casos no sean negativos
	
	bool numeroNegativo= false;
	if(numeroIntroducidoPorUsuario <= 0)
		numeroNegativo= true;
	return numeroNegativo;
}

double convertirNumeroANegativo(double valorUsuario){
	
	// Este bloque de código sólo ejecuta cuando el usuario inserta los valores de la fila Z, los convierte todos a negativos
	
	double numeroConvertido;
	numeroConvertido= valorUsuario*-1;
	return numeroConvertido;
}
