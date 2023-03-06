#include "control.h"
#include <QDebug>


//imgMatrix jest macierzą do zmaiany
Control::Control(QObject *parent): QObject(parent) {

}

void Control::getImageMatrix(float **f, int rows, int cols) {
    imgMatrix = f;
    rows_ = rows;
    cols_ = cols;
}



void Control::dofil(float **imgin, float **imgout, int mask[3][3])
{
    float copyimg[100][100];
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                    copyimg[i][j]=imgin[i][j];
            }
    }
    int sumask=0;
    for (int i = 0; i <3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            sumask=sumask+mask[i][j];
        }
    }
    //qDebug()<<"suma mask: "<<sumask;
if(sumask<1) sumask = 1;
int suma=0;
    for (int i = 1; i <99; i++)
    {
            for(int j = 1; j < 99; j++)
            {
                suma=0;
                for (int k = 0; k <3; k++)
                {
                    for(int l = 0; l < 3; l++)
                    {
                        suma=suma+copyimg[i+k-1][j+l-1]*mask[k][l];
                    }
                }
             //   qDebug()<<"suma mask: "<<sumask;
                imgout[i][j]=suma/sumask;
            }
    }
}

void Control::bspline(float **imgin, float **imgout)
{
    float copyimg[100][100];
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                    copyimg[i][j]=imgin[i][j];
            }
    }



    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                imgMatrix[i][j]=255;
            }
    }

    int countery=0;
    for (int i = 48; i <=58; i++)
    {
        int counterx=0;
        for (int j = 48; j <=58; j++)
        {
            imgMatrix[counterx][countery] =copyimg[j][i];

            counterx=counterx+10;
            if(counterx==100){
                counterx=99;
            }

    }
        countery=countery+10;
        if(countery==100){
            countery=99;
        }
    }

    float pl, pp, wp, wl;
    for (int j = 0; j <=100; j+=10)
    {
    for (int i = 0; i <100; i++)
    {
        pl=imgMatrix[i-i%10][(j>99? 99:j)];
        pp=imgMatrix[(i-i%10+10>99? 99:i-i%10+10)][(j>99? 99:j)];
        wp=i%10/10.;
        wl=1-i%10/10.;

        imgMatrix[i][(j>99? 99:j)] = pl*wl+pp*wp;
    }
    }

    for (int i = 0; i <=100; i++)
    {
    for (int j = 0; j <100; j++)
    {
        pl=imgMatrix[(i>99? 99:i)][(j-j%10)];
        pp=imgMatrix[(i>99? 99:i)][(j-j%10+10>99? 99:j-j%10+10)];
        wp=j%10/10.;
        wl=1-j%10/10.;

        imgMatrix[i>99? 99:i][j] = pl*wl+pp*wp;
    }
    }

}

void Control::hist(float **imgin, float **imgout)
{
    int tmin,tmax;
    tmin=imgMatrix[0][0];
    tmax=imgMatrix[0][0];
    for (int i = 0; i <100; i++)
    {
        for (int j = 0; j <100; j++)
        {
        if (imgMatrix[i][j] < tmin)
        {
            tmin = imgMatrix[i][j];
        }
    }
    }

    for (int i = 0; i <100; i++)
    {
        for (int j = 0; j <100; j++)
        {
            if (imgMatrix[i][j] > tmax)
            {
                tmax = imgMatrix[i][j];
            }
        }
    }
    qDebug()<<"tmax="<<tmax<<"       tmin="<<tmin;

    int lookuptab[256];
    for (int i = tmin; i <=tmax; i++)
    {
        lookuptab[i]=(i-tmin)*255/(tmax-tmin);
        //qDebug()<<"lookuptab = "<<lookuptab[i]<<"   "<<i;
    }


    for (int i = 0; i <100; i++)
    {
        for (int j = 0; j <100; j++)
        {
            imgMatrix[i][j]=lookuptab[(int)imgMatrix[i][j]];

        }
    }
int t = 0;
    int count[256];
    for (int i = 0; i <255; i++)
    {count[i]=0;
    }

    for(int k = 0; k <255; k++){
    for (int i = 0; i <100; i++)
    {
        for (int j = 0; j <100; j++)
        {

         t=   imgMatrix[i][j]/3 ;
         if(t==k){
            count[k]++;
         }
        }
    }
    // qDebug()<<count[k]<<"  ";
}


int kmax= count[0];

for(int k = 0; k <255; k++){

if(count[k]>kmax){
    kmax=count[k];
}

}


    for (int i = 0; i <100; i++)
    {
    for (int j = 0; j <100; j++)
    {
        imgMatrix[i][j]=255;
    }
    }

    for (int k = 0; k <100; k++)
    {

        count[k]=count[k]/(kmax/100);


    }
        qDebug()<<kmax;





    int len =0;

    for (int i = 0; i <100; i++)
    {

      len=  count[i];
      //qDebug()<<len<<"  ";
    for (int j =99; j >=100-len; j--)
    {
       imgMatrix[i][j]=0;
    }
    }





//4138 == 100pix
///41,3 - 1 pix







}

void Control::census(float **imgin, int i, int j, bool *tab1)
{

     if(imgMatrix[i][j]<=imgMatrix[i][j-1]) tab1[0]=1; else tab1[0]=0;
        if(imgMatrix[i][j]<= imgMatrix[i+1][j-1]  )      tab1[1]=1; else tab1[1]=0;
       if(imgMatrix[i][j]<=  imgMatrix[i+1][j]   )         tab1[2]=1; else tab1[2]=0;
        if(imgMatrix[i][j]<= imgMatrix[i+1][j+1]  )     tab1[3]=1; else tab1[3]=0;
        if(imgMatrix[i][j]<= imgMatrix[i][j+1]   )       tab1[4]=1; else tab1[4]=0;
      if(imgMatrix[i][j]<=  imgMatrix[i-1][j+1]   )    tab1[5]=1; else tab1[5]=0;
      if(imgMatrix[i][j]<=  imgMatrix[i-1][j]    )     tab1[6]=1; else tab1[6]=0;
        if(imgMatrix[i][j]<= imgMatrix[i-1][j-1]  )        tab1[7]=1; else tab1[7]=0;

}

int Control::distHan(bool *tab, bool *tab1,bool *tab2)
{
    int count=0;
    if(tab[0]==tab1[0]) tab2[0]=0; else{ tab2[0]=1;count++;}
    if(tab[1]==tab1[1]) tab2[1]=0; else{ tab2[1]=1;count++;}
    if(tab[2]==tab1[2]) tab2[2]=0; else {tab2[2]=1;count++;}
    if(tab[3]==tab1[3]) tab2[3]=0; else {tab2[3]=1;count++;}
    if(tab[4]==tab1[4]) tab2[4]=0; else{ tab2[4]=1;count++;}
    if(tab[5]==tab1[5]) tab2[5]=0; else{ tab2[5]=1;count++;}
    if(tab[6]==tab1[6]) tab2[6]=0; else {tab2[6]=1;count++;}
    if(tab[7]==tab1[7]) tab2[7]=0; else {tab2[7]=1;count++;}

    return count;

}

void Control::dylatacja(float **imgin, float **imgout, bool element[3][3])
{

    float copyimg[100][100];
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                    copyimg[i][j]=imgin[i][j];
            }
    }
    bool domorf;

    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                domorf=false;
                for (int k=0;k<3;k++){
                    for(int l=0;l<3;l++){
                        if (element[k][l])
                        {
                            if(copyimg[(i-1+k)<0 ? 0: (i-1+k) > 99 ? 99: i-1+k][(j-1+l)<0 ? 0: (j-1+l) > 99 ? 99: j-1+l]>128)
                            {
                                domorf=true;
                            }
                        }
                    }
                }
                if (domorf) imgout[i][j]=255;
                else imgout[i][j]=0;
            }
    }
}

void Control::erozja(float **imgin, float **imgout, bool element[3][3])
{

    float copyimg[100][100];
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                    copyimg[i][j]=imgin[i][j];
            }
    }
    bool domorf;
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                domorf=false;
                for (int k=0;k<3;k++){
                    for(int l=0;l<3;l++){
                        if (element[k][l])
                        {
                            if(copyimg[(i-1+k)<0 ? 0: (i-1+k) > 99 ? 99: i-1+k][(j-1+l)<0 ? 0: (j-1+l) > 99 ? 99: j-1+l]<128)
                            {
                                domorf=true;
                            }
                        }
                    }
                }
                if (domorf) imgout[i][j]=0;
                else imgout[i][j]=255;
            }
    }
}

void Control::roznica(float **imgin, float **imgin2, float **imgout)
{
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                imgout[i][j]=imgin[i][j]-imgin2[i][j];
            }
    }
}

void Control::suma(float **imgin, float **imgin2, float **imgout)
{
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                imgout[i][j]=imgin[i][j]+imgin2[i][j];
            }
    }
}

int Control::pusty(float **imgin)
{

    for (int i = 0; i <100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            if(imgin[i][j]>0){
                return 0;

            }
        }


    }
    return 1;
}

void Control::mul(float **imgin, float **imgin2, float **imgout)
{
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                imgout[i][j]=imgin[i][j]*imgin2[i][j];
            }
    }
}





void Control::doSkeleton(){

    //incijalziacja tablic

        float ** img = new float*[100];
        float ** img1 = new float*[100];
        float ** img2 = new float*[100];
        float ** img3 = new float*[100];
        float ** img4 = new float*[100];
        for (int i = 0;i<100;i++)
        {
            img[i]=new float [100];
             img1[i]=new float [100];
              img2[i]=new float [100];
               img3[i]=new float [100];
               img4[i]=new float [100];
        }

        //float copyimg[100][100];
        for (int i = 0; i <100; i++)
        {
                for(int j = 0; j < 100; j++)
                {
                        img[i][j]=imgMatrix[i][j];
                        img4[i][j]=0;
                }
        }
//szkieletwowanie
//1.erozja; 2. otwarcie 3. roznica 1-2 4. suma 3 5. sprawdza czy 1 pusty
        bool element[3][3]={{0,1,0},{1,1,1},{0,1,0}};

        do {
        //open
        erozja(img,img1,element);
        dylatacja(img1,img2,element);
        //roznica
        roznica(img,img2,img3);
        //suma
        suma(img3,img4,img4);

        erozja(img,img,element);
        //pusty

        pusty(img);
        }
        while(pusty(img)==0);

        for (int i = 0; i <100; i++)
        {
                for(int j = 0; j < 100; j++)
                {
                        imgMatrix[i][j]=img4[i][j];
                }
        }




    //usuwanie tablic
            for (int i = 0;i<100;i++)
        {
            delete[] img[i];
                delete[] img1[i];
                delete[] img2[i];
                delete[] img3[i];
                delete[] img4[i];
        }
            delete[] img;
            delete[] img1;
            delete[] img2;
            delete[] img3;
            delete[] img4;
}

void Control::doCage() {

   bool pattern[8];
   bool tab1[8];
   bool tab2[8];
   int count=0;

   float imgDistance[100][100];
   for (int i = 0; i <100; i++)
    for (int j = 0; j <100; j++)
        imgDistance[i][j]=255;


   census(imgMatrix, 1,1 ,pattern);
    census(imgMatrix, 2,3 ,tab1);

   qDebug()<<pattern[0]<<pattern[1]<<pattern[2]<<pattern[3]<<pattern[4]<<pattern[5]<<pattern[6]<<pattern[7];
   count = distHan(pattern,tab1,tab2);

    qDebug()<<tab1[0]<<tab1[1]<<tab1[2]<<tab1[3]<<tab1[4]<<tab1[5]<<tab1[6]<<tab1[7];
   qDebug()<<tab2[0]<<tab2[1]<<tab2[2]<<tab2[3]<<tab2[4]<<tab2[5]<<tab2[6]<<tab2[7]<<count;


   for (int i = 1; i <99; i++)
   {
       for (int j = 1; j <99; j++)
       {
         census(imgMatrix, i,j,tab1);
        imgDistance[i][j]=distHan(pattern,tab1,tab2);

   }
   }

   for (int i = 1; i <99; i++)
   {
       for (int j = 1; j <99; j++)
       {
      imgMatrix[i][j]=  30*(imgDistance[i][j]);

   }
   }
  int imgMatrixx[100][100];
   for (int i = 1; i <99; i++)
   {
       for (int j = 1; j <99; j++)
       {
      imgMatrixx[i][j]=  imgMatrix[i][j];

   }
   }

   for (int i = 1; i <99; i++)
   {
       for (int j = 1; j <99; j++)
       {
      imgMatrix[i][j]=  255;

   }
   }


   for (int i = 1; i <99; i++)
   {
       for (int j = 1; j <99; j++)
       {
           if(imgDistance[i][j]<1){
imgMatrix[i-1][j-1]=  imgMatrixx[i-1][j-1];
imgMatrix[i-1][j]= imgMatrixx[i-1][j];
imgMatrix[i-1][j+1]= imgMatrixx[i-1][j+1];
imgMatrix[i][j-1]=  imgMatrixx[i][j-1];
imgMatrix[i][j]=  imgMatrixx[i][j];
imgMatrix[i][j+1]=  imgMatrixx[i+1][j+1];
imgMatrix[i+1][j-1]=  imgMatrixx[i+1][j-1];
imgMatrix[i+1][j]=  imgMatrixx[i+1][j];

      imgMatrix[i+1][j+1]=  imgMatrixx[i+1][j+1];

           }

   }
   }

}


void Control::doConnectedComponent() {
    int ile=4;
    for(int j = 0; j < rows_; j++) {
            for(int i = 0; i <cols_; i++) {

                if(imgMatrix[i][j] > 0){

                if (imgMatrix[i-1][j] > 0)
                {
                    imgMatrix[i][j] =   imgMatrix[i-1][j] ;
                }
                else if (imgMatrix[i][j-1] > 0)
                {
                    imgMatrix[i][j] =   imgMatrix[i][j-1] ;
                }
                else
                {
                    ile++;
                    imgMatrix[i][j] = ile;
           }
           }
            }
    }
             int COR[256];
             for( int i=0;i<256;i++){

               COR[i]=i;
}
             for(int j = 0; j < rows_; j++) {
                     for(int i = 0; i <cols_; i++) {
                         if(imgMatrix[i][j-1]>0 && imgMatrix[i][j] >0) {
               if (imgMatrix[i][j-1] < imgMatrix[i][j] )
               {
                   COR[(int)imgMatrix[i][j]]=COR[(int)imgMatrix[i][j-1]];
               }
               else if (imgMatrix[i][j-1] > imgMatrix[i][j] )
               {
                   COR[(int)imgMatrix[i][j-1]]=COR[(int)imgMatrix[i][j]];
                    imgMatrix[i][j]=COR[i];
               }
               }
                     }

}
             for(int j = 0; j < rows_; j++) {
                     for(int i = 0; i <cols_; i++) {
                         imgMatrix[i][j]= COR[(int)imgMatrix[i][j]];
                     }
}
}

void Control::doPrzycisk()
{
    bool element[3][3]={{0,1,0},{1,1,1},{0,1,0}};
    erozja(imgMatrix,imgMatrix,element);
}

void Control::doPrzycisk1()
{
    bool element[3][3]={{0,1,0},{1,1,1},{0,1,0}};
    dylatacja(imgMatrix,imgMatrix,element);
}

void Control::doOpen()
{
    bool element[3][3]={{0,1,0},{1,1,1},{0,1,0}};
    erozja(imgMatrix,imgMatrix,element);
    dylatacja(imgMatrix,imgMatrix,element);
}

void Control::doClose()
{
    bool element[3][3]={{0,1,0},{1,1,1},{0,1,0}};
    dylatacja(imgMatrix,imgMatrix,element);
    erozja(imgMatrix,imgMatrix,element);
}

void Control::doHarris()
{
    float ** img = new float*[100];
    float ** img1 = new float*[100];
    float ** img2 = new float*[100];
    float ** img3 = new float*[100];
    float ** img4 = new float*[100];
    float ** img5 = new float*[100];
    for (int i = 0;i<100;i++)
    {
        img[i]=new float [100];
         img1[i]=new float [100];
          img2[i]=new float [100];
           img3[i]=new float [100];
           img4[i]=new float [100];
           img5[i]=new float [100];
    }

    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                    img[i][j]=imgMatrix[i][j];
                    //img4[i][j]=0;
            }
    }

    //ALGORYTM HARRISA
    int maskfx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}}; //FX
    int maskfy[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}}; //FY
    int maskg[3][3]={{1,2,1},{2,4,2},{1,2,1}}; //Gauss

    dofil(imgMatrix,img1,maskfx);
    dofil(imgMatrix,img2,maskfy);

    mul(img1,img1,img3); //XX
    mul(img1,img2,img4); //XY
    mul(img2,img2,img5); //YY

    dofil(img3,img3,maskg); //IXIX
    dofil(img4,img4,maskg); //IXIY
    dofil(img5,img5,maskg); //IYIY


float R;
float det;
float trace;

        for(int i = 0; i < 100; i++)
        {
            for(int j = 0; j < 100; j++)
            {
                det=img3[i][j]*img5[i][j]-(img4[i][j]*img4[i][j]);
                trace=img3[i][j]+img5[i][j];
                R=(det-0.05*trace*trace)/500000000;
                qDebug()<<"R = "<<R;
                    imgMatrix[i][j]=img2[i][j];
                if(R>255)
                    imgMatrix[i][j]=255;

                else if (R<0)
                 imgMatrix[i][j]=0;
                else
                    imgMatrix[i][j]=R;
            }
        }
    //ALGORYTM HARRISA

    for (int i = 0;i<100;i++)
{
    delete[] img[i];
        delete[] img1[i];
        delete[] img2[i];
        delete[] img3[i];
        delete[] img4[i];
        delete[] img5[i];
}
    delete[] img;
    delete[] img1;
    delete[] img2;
    delete[] img3;
    delete[] img4;
    delete[] img5;
}

void Control::doLin()
{
    int rho=0;


    int acc[180][289];
    for (int i = 0; i <180; i++)
    {
            for(int j = 0; j < 289; j++)
            {
                    acc[i][j]=0;
            }
    }

    for (int i = 0; i <180; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            {
                for(int k = 0;k<100;k++){

                    if(imgMatrix[j][k]!=0){
                        rho=j*cos(i)+k*sin(i)+144;
                        if(abs(rho)>=2){
                        acc[i][rho]++;
                        }
                    }
                }
            }
        }
    }

    int max=0;
    int x=0;
    int y=0;

    for (int i = 0; i <180; i++)
    {
            for(int j = 0; j < 289; j++)
            {
                if(max<acc[i][j]){
                  max=  acc[i][j];
                x=i;
                y=j;
                }
            }
    }
    qDebug()<<"theta: "<<x<<"  rho:  "<<y<<"  countRho:  "<<max;
//rysuj prostą
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                rho=i*cos(x)+j*sin(x)+144;
                if(rho==y){
                imgMatrix[i][j]=140;

                }
            }
    }
}

void Control::doLast()
{
    float ** img = new float*[100];
    float ** img1 = new float*[100];
    float ** img2 = new float*[100];
    float ** img3 = new float*[100];

    for (int i = 0;i<100;i++)
    {
        img[i]=new float [100];
        img1[i]=new float [100];
        img2[i]=new float [100];
        img3[i]=new float [100];
    }

    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                    img[i][j]=imgMatrix[i][j];
            }
    }

    bool maskB1[3][3]={{0,1,0},{0,1,1},{0,0,0}}; //B1
    bool maskB2[3][3]={{1,0,0},{1,0,0},{1,1,1}}; //B2

    bool maskR1[3][3]={{0,0,0},{0,1,0},{0,0,0}}; //B1
    bool maskR2[3][3]={{0,1,0},{1,0,0},{0,1,0}}; //B1

     bool maskR3[3][3]={{0,0,0},{0,1,0},{0,0,0}}; //B1
    bool maskR4[3][3]={{0,1,0},{1,0,1},{0,0,0}}; //B1

     bool maskR5[3][3]={{0,0,0},{0,1,0},{0,0,0}}; //B1
    bool maskR6[3][3]={{0,1,0},{0,0,1},{0,1,0}}; //B1

     bool maskR7[3][3]={{0,0,0},{0,1,0},{0,0,0}}; //B1
    bool maskR8[3][3]={{0,0,0},{1,0,1},{0,1,0}}; //B1





//odwrocenie obrazu A^c
    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                if(imgMatrix[i][j]>128)
                    img2[i][j]=0;
                else
                    img2[i][j]=255;
            }
    }


    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                imgMatrix[i][j]=0;
            }
    }

    erozja(img,img1,maskR1);
    erozja(img2,img3,maskR2);

    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                if((img1[i][j]>0)&&(img3[i][j]>0))
                {
                imgMatrix[i][j]=255;
                }
            }
    }

    erozja(img,img1,maskR3);
    erozja(img2,img3,maskR4);

    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                if((img1[i][j]>0)&&(img3[i][j]>0))
                {
                imgMatrix[i][j]=255;
                }
            }
    }

    erozja(img,img1,maskR5);
    erozja(img2,img3,maskR6);

    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                if((img1[i][j]>0)&&(img3[i][j]>0))
                {
                    imgMatrix[i][j]=255;
                }
            }
    }

    erozja(img,img1,maskR7);
    erozja(img2,img3,maskR8);

    for (int i = 0; i <100; i++)
    {
            for(int j = 0; j < 100; j++)
            {
                if((img1[i][j]>0)&&(img3[i][j]>0))
                {
                    imgMatrix[i][j]=255;
                }
            }
    }



    for (int i = 0;i<100;i++)
{
        delete[] img[i];
        delete[] img1[i];
        delete[] img2[i];
        delete[] img3[i];
}
    delete[] img;
    delete[] img1;
    delete[] img2;
    delete[] img3;
}

void Control::doFilter()
{
        int mask[3][3]={{-1,0,1},{-1,0,2},{-1,0,1}};
        dofil(imgMatrix, imgMatrix,mask);
}

void Control::doHist()
{
    hist(imgMatrix, imgMatrix);
}

void Control::doBSP()
{
    bspline(imgMatrix, imgMatrix);
}


