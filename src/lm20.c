/*
 * lm20.c
 *
 * Created: 3/19/2017 1:16:36 PM
 *  Author: denis
 */ 

#include "lm20.h"
#include "lm20_config.h"

/*tabelul valorilor
(temperatura:voltaj)*/
int myLookUpTable[9][2]=
{
	{130,303},
	{100,675},
	{80,919},
	{30,1515},
	{25,1574},
	{0,1864},
	{-30,2205},
	{-40,2318},
	{-55,2485},
};


/*
convertarea valorii ADC
in Volataj.
Param.IN:ADC value;Type-int
Param.OUT:Voltage-value;Type-int
*/
int To_Volts(int val)
{
	return ((long int)val * ADC_VOLTAGE) / ADC_RESOLUTION;
}

/*Determinarea temperaturii
Param.IN-
1)Tensiunea
2)metoda utilizata(daca '1' efectueaza metoda
prin analiza intervalelor,daca orice alta val.
atunci efectueza calculul dupa expresia din datasheet
Param.OUT-Temperatura*/
int To_Temp(int voltageValue,char method)
{
	int tmp,i;
	int dTemp;
	float dVoltage;

	for(i=0; i < 8; i++)
	{
		if(i==0)
		tmp=myLookUpTable[i][0];


		if(voltageValue-1 < myLookUpTable[i][1])
		{
			tmp = myLookUpTable[i][0];
			//return tmp;
			break;
		}
	}
	if(i==8) tmp=myLookUpTable[i][0];
	
	//a2 parte,cercetam segmentul obtinut
	if(method == 1)
	tmp = BisectMethod(voltageValue,myLookUpTable[i-1][1],myLookUpTable[i][1],myLookUpTable[i-1][0],myLookUpTable[i][0]);
	else
	tmp = FunctionalMethod(voltageValue);
	return tmp;
}

/*
Functia data cerceteaza intervalul de temperaturi
p/u gasirea temp. precise.
Param. IN-
1)volajul de la senzor
2),3)valorile voltajului la extremele intervalului pozitia(i-1,i)
4),5)valorile voltajului la extremele intervalului pozitia(i-1,i)
In caz de eroare returneaza '1000';
Param.OUT-Temperatura(int);
*/
int BisectMethod(int vV,int v1,int v2,int t1,int t2)
{
	float vMed,tMed;

	vMed = v1;
	tMed = t1;

	// |vMed-vV| > 10 - eroare 10mV
	while(abs(vMed-vV) > 10)
	{
		//Injumatatirea segmentului
		vMed = (v1 + v2) / 2;
		tMed = (t1 + t2) / 2;
		/*Gasirea extremelor a segm. injumatatit*/
		if(vV > vMed) {
			v1 = vMed;
			t1 = tMed;
		}
		else if(vV < vMed) {
			v2 = vMed;
			t2 = tMed;
		}
		else if(vV == vMed) {
			return tMed;
		}
		else return 1000;//error
	}
	return tMed;
}


/*Expresia din lm20 datasheet p/t
calcularea temperaturii
Param.In-Voltajul
Param.OUT-Temperatura*/
int FunctionalMethod(int vV)
{
	return (-1481.96f + sqrt(2.1962f * 1000000u + (1.8639f - (float)vV/1000)/(3.88f * 0.000001f)));
}
