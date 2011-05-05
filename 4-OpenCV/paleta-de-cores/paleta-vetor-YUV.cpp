/*
  This program executes the conversion of a stereo pair of images into an anaglyph
  image as well as the reversion, from an anaglyph image to a stereo pair. The
  stereo pair can be of two types: side-by-side or above below. In the reversion
  process, an auxiliary vector is used, which contains channels that were disposed 
  in the anaglyph process. The anaglyph image is converted to YUV color space with
  4:2:2 subsampling, reducing the amount of information. We generate a gree/magenta
  anaglyph image

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: April, 30th 2011
  Last modification: May, 5th 2011
  
  Usage:
        paleta-imagem <image.bmp> {-sbs | -ab} {-e | -d}
        
        -sbs: side-by-side image
        -ab: above/below image
        -e: encoding
        -d: decoding
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

/* 
   Given a float number, returns a character with the number rounded. 
   Input: num - float number
   Output: rounded number stored in a signed char type
*/
uchar roundFloat(float num){
     int round = (int)num;
     if(num - round > 0.5)//rounds to ceil
            return (uchar)num+1;
     else return (uchar)num;
}

/* Convert an image from YUV to RGB format, using the following formula
   YUV to RGB:
       R = Y + 1.403f * V
       G = Y - 0.344f * U - 0.714f * V
       B = Y + 1.77f * U
   Input: yuvImg - file containg YUV data
*/
void YUVtoBGR(char *yuvImg){
     FILE *fp;
     int width, height;
     IplImage *bgrImg;
     uchar *vet; //holds YUV data
    
     //get image width and height from YUV file
     fp = fopen(yuvImg, "rb");    
     fscanf(fp, "%d %d", &width, &height);
     int imageSize = width*height;
     
     //We will create a 24bits RGB image
     CvSize size = cvSize(width, height);
     bgrImg = cvCreateImage(size, IPL_DEPTH_8U, 3);
     cvZero(bgrImg);
     
     //get YUV data and convert to RGB
     vet = (uchar*)malloc(3*imageSize*sizeof(uchar));
     fread(vet, sizeof(uchar), 3*imageSize, fp);
     printf("Convertendo de RGB para YUV... ");
     int count = 0;
     /* Fórmula do Léo
     for(int row = 0; row < bgrImg->height; row++){
        //set pointer to the correct position in each row
        uchar* ptr = (uchar*)(bgrImg->imageData + row * bgrImg->widthStep);
        for(int col = 0; col < bgrImg->width; col++){
            // Y -> first imageSize elements of vet -> vet[count]
            // U -> next imageSize elements of vet ->  vet[count+imageSize]
            // V -> last imageSize elements of vet ->  vet[count+2*imageSize]
            ptr[3*col+2] = roundFloat(vet[count] + 1.403f * vet[count+2*imageSize]); //R
            ptr[3*col+1] = roundFloat(vet[count] - 0.344f * vet[count+imageSize] - 0.714f * vet[count+2*imageSize]);//G
            ptr[3*col] = roundFloat(vet[count] + 1.77f * vet[count+imageSize]);//B
            count++;
        }
     }*/
     
     /*test using OpenCV function
     for(int row = 0; row < bgrImg->height; row++){
        //set pointer to the correct position in each row
        uchar* ptr = (uchar*)(bgrImg->imageData + row * bgrImg->widthStep);
        for(int col = 0; col < bgrImg->width; col++){
            ptr[3*col+2] = (uchar)vet[count+2*imageSize]; //R
            ptr[3*col+1] = (uchar)vet[count+imageSize];//G
            ptr[3*col] = (uchar)vet[count];//B
            count++;
        }
     }
     cvCvtColor(bgrImg, bgrImg, CV_YCrCb2BGR);    
     */ 
     
     // Using openCV formula
     for(int row = 0; row < bgrImg->height; row++){
        //set pointer to the correct position in each row
        uchar* ptr = (uchar*)(bgrImg->imageData + row * bgrImg->widthStep);
        for(int col = 0; col < bgrImg->width; col++){
            // Y -> first imageSize elements of vet -> vet[count]
            // U -> next imageSize elements of vet ->  vet[count+imageSize]
            // V -> last imageSize elements of vet ->  vet[count+2*imageSize]
            ptr[3*col+2] = roundFloat(vet[count] + 1.403f * (vet[count+2*imageSize] - 128)); //R
            ptr[3*col+1] = roundFloat(vet[count] - 0.344f * (vet[count+imageSize] - 128) - 0.714f * (vet[count+2*imageSize] - 128));//G
            ptr[3*col] = roundFloat(vet[count] + 1.77f * (vet[count+imageSize] - 128));//B
            count++;
        }
     }
     
     cvSaveImage("anaglyphYUV2RGB.bmp", bgrImg);
     cvReleaseImage(&bgrImg);
     free(vet);
     
}

/* 
   Convert an image from RGB to YUV format, using the following formula
   RGB TO YUV:
       Y = 0.299f * R + 0.587 * G + 0.114 * B
       U = 0.565f * (B - Y);
       V = 0.713f * (R - Y);
   Image is saved in a binary file containing signed char elements
   Input: img - image to be converted
*/
void BGRtoYUV(IplImage *img){
    uchar *yuvImg;
    int imageSize = img->width * img->height;
    FILE *fp;
    
    //allocate vector containing YUV data
    //first imageSize positions holds Y data
    //next imageSize positions holds U data
    //last imageSize positions holds V data
    yuvImg = (uchar*) malloc(3*imageSize*sizeof(uchar));
        
    //RGB to YUV conversion
    printf("Convertendo de RGB para YUV... ");
    int count = 0;
    /* fórmula do Léo
    for(int row = 0; row < img->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrAn = (uchar*)(img->imageData + row * img->widthStep);
        for(int col = 0; col < img->width; col++){
            float Y = 0.299f * ptrAn[3*col+2] + 0.587f * ptrAn[3*col+1] + 0.114f * ptrAn[3*col];
            yuvImg[count] = roundFloat(Y); //Y
            yuvImg[count+imageSize] = roundFloat(0.565f * (ptrAn[3*col] - Y)); //U
            yuvImg[count+2*imageSize] = roundFloat(0.713f * (ptrAn[3*col+2] - Y)); //V
            count++;
        }
    }*/
    
    /*test using openCV function
    IplImage *result;
    CvSize size = cvGetSize(img);
    result = cvCreateImage(size, img->depth, img->nChannels);
    cvZero(result);
    cvCvtColor(img, result, CV_BGR2YCrCb);
    for(int row = 0; row < result->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrAn = (uchar*)(result->imageData + row * result->widthStep);
        for(int col = 0; col < result->width; col++){
            yuvImg[count] = (uchar)ptrAn[3*col]; //Y
            yuvImg[count+imageSize] = (uchar)ptrAn[3*col+1];//U
            yuvImg[count+2*imageSize] = (uchar)ptrAn[3*col+2]; //V
            count++;
        }
    }*/
        
    //OpenCV Formula
    for(int row = 0; row < img->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrAn = (uchar*)(img->imageData + row * img->widthStep);
        for(int col = 0; col < img->width; col++){
            float Y = 0.299f * ptrAn[3*col+2] + 0.587f * ptrAn[3*col+1] + 0.114f * ptrAn[3*col];
            yuvImg[count] = roundFloat(Y); //Y
            yuvImg[count+imageSize] = roundFloat(0.564f * (ptrAn[3*col] - Y) + 128); //U
            yuvImg[count+2*imageSize] = roundFloat(0.713f * (ptrAn[3*col+2] - Y) + 128); //V
            count++;
        }
    }
    
    //save YUV file
    fp = fopen("anaglyph.yuv","wb");
    if(!fp){
        printf("Erro na criacao de arquivo de imagem YUV. Favor verificar\n");
        exit(-1);
    }
    //first data from file have the image's width and height
    fprintf(fp, "%d %d", img->width, img->height);
    fwrite(yuvImg, sizeof(uchar),3*imageSize, fp);
    
    fclose(fp);
    free(yuvImg);
    printf("OK!\n");
}

/*
  Separate images from a side-by-side or above/below image. Given the original
  image dimensions, set a ROI (region of interest) based on the image type. If
  it's side-by-side, the ROI will be half of the original image's width. If it's
  above-below, the ROI will be half of the original image's height. Only the pixels
  inside the ROI will be copied by cvCopy
  Input: frame - original image
           frameL, frameR - stores left and right image, respectively
           width, height - original image dimensions
           imageType - -sbs if the original image has side-by-side images, -ab
            if it has above-below images                
*/
void separateImages(IplImage *frame, IplImage **frameL, IplImage **frameR, int width, int height, char *imageType){    
    printf("Separando Imagens... ");
    if(!strcmp(imageType,"-sbs")){
        //set ROI to cut the image on the left (half of the original)
        cvSetImageROI(frame, cvRect(0,0, width/2, height));            
        //copy image portion
        cvCopy(frame, *frameL);            
        //reset ROI
        cvResetImageROI(frame);            
        //set ROI to cut the image on the right (second half of the original)
        cvSetImageROI(frame, cvRect(width/2, 0, width/2, height));            
        //copy image portion
        cvCopy(frame, *frameR);            
        //reset ROI
        cvResetImageROI(frame);
    }
    else if(!strcmp(imageType, "-ab")){
        //set ROI to cut the image on the top (half of the original)
        cvSetImageROI(frame, cvRect(0,0, width, height/2));            
        //copy image portion
        cvCopy(frame, *frameL);            
        //reset ROI
        cvResetImageROI(frame);            
        //set ROI to cut the image on the bottom (second half of the original)
        cvSetImageROI(frame, cvRect(0, height/2, width, height/2));            
        //copy image portion
        cvCopy(frame, *frameR);            
        //reset ROI
        cvResetImageROI(frame);
    }  
    printf("OK!\n");
}

/*
  Get an anaglyph image and create the stereo pair, using an auxiliary image to
  get channels disposed in the anaglyph process.
  Input: frame - anaglyph image
           imageType - -sbs to create a side-by-side image or -ab to create a above-below one           
*/
void revertAnaglyph(char *yuvImg, char *imageType){
    /*IplImage *stereoPair;
    
    //new image will have double width or double height, depending on the image type
    CvSize size;
    if(!strcmp(imageType,"-sbs")){
        size = cvSize( width*2, height);
    }
    else if(!strcmp(imageType, "-ab")){
        size = cvSize( width, height*2);
    }
    
    //We will create a RGB 24bits image
    stereoPair = cvCreateImage(size, IPL_DEPTH_8U, 3);
    cvZero(stereoPair);
    
    //get data from disposed channels
    fp = fopen("pixelData.dat", "rb");       
    uchar *vet = (uchar*)malloc(3*imageSize);
    fread(vet, sizeof(uchar), 3*imageSize, fp);
    fclose(fp);
    
    
    printf("Revertendo imagem anaglifica para par estereo... ");
    int count=0;
    if(!strcmp(imageType, "-sbs")){    
        for(int row = 0; row < frame->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
            uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
            for(int col = 0; col < frame->width; col++){
                // Copy values from frame (R and B channels) and vet (G 
                // channel) to stereoPair for the first half of image width
                ptrStp[3*col] = ptrAn[3*col];
                ptrStp[3*col+2] = ptrAn[3*col+2];
                ptrStp[3*col+1] = vet[count];
                // Copy values from frame (G channel) and vet (R and B 
                // channels) to stereoPair for the rest of image width
                ptrStp[3*(col+frame->width)] = vet[count+(2*imageSize)];
                ptrStp[3*(col+frame->width)+2] = vet[count+imageSize];
                ptrStp[3*(col+frame->width)+1] = ptrAn[3*col+1];
                
                count++;
            }
        }
    }
    else if(!strcmp(imageType, "-ab")){
        for(int row = 0; row < frame->height; row++){
             //set pointer to the correct position in each row
             uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
             uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
             for(int col = 0; col < frame->width; col++){
                 // Copy values from frame (R and B channels) and vet (G 
                 // channel) to stereoPair for the first half of image height 
                 ptrStp[3*col] = ptrAn[3*col];
                 ptrStp[3*col+1] = vet[count];
                 ptrStp[3*col+2] = ptrAn[3*col+2];
                 
                 count++;
             }
        }
        count = 0;
        for(int row = 0; row < frame->height; row++){
             //set pointer to the correct position in each row
             uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
             uchar* ptrStp = (uchar*)(stereoPair->imageData + (row + frame->height) * stereoPair->widthStep);
             for(int col = 0; col < frame->width; col++){
                 // Copy values from frame (G channel) and vet (R and B 
                 // channels) to stereoPair for the rest of image height
                 ptrStp[3*col] = vet[count+(2*imageSize)]; //blue
                 ptrStp[3*col+2] = vet[count+imageSize]; //red
                 ptrStp[3*col+1] = ptrAn[3*col+1];
                 
                 count++;
             }
        }
     }
     cvSaveImage("Par_Estereo.bmp",stereoPair);
     printf("Ok!\n");       
     cvReleaseImage(&stereoPair);
     free(vet);*/
}

/*
  Get the stereo pair and create the green-magenta anaglyph. Copy the removed channels
  to an auxiliary file to be reused in order to make the reverse process (from 
  anaglyph to stereo pair, without any losses).
  Input: frameL, frameR - stores left and right image, respectively
*/
void createAnaglyph(IplImage *frameL, IplImage *frameR){
    IplImage *anaglyph; 
    uchar *disposed_channels; //hold channels lost during anaglyph transformation
    
    //prepare anaglyph image
    CvSize size = cvGetSize(frameL);
    anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(anaglyph);
    
    //prepare vector
    int imageSize = frameL->width*frameL->height;
    disposed_channels = (uchar*)malloc(3*imageSize);
    
    //anaglyph transformation
    printf("Criando imagem anaglifica... ");
    int count = 0;
    for(int row = 0; row < frameL->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrR = (uchar*)(frameR->imageData + row * frameR->widthStep);
            uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
            uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
            for(int col = 0; col < frameL->width; col++){
                    //copy green channel from the right image and red and blue channel from the left image
                    ptrA[3*col+1] = ptrR[3*col+1];
                    ptrA[3*col] = ptrL[3*col];
                    ptrA[3*col+2] = ptrL[3*col+2];
                    //copy remain channels to their corresponding vectors
                    disposed_channels[count] = ptrL[3*col+1]; //green
                    disposed_channels[count+imageSize] = ptrR[3*col+2]; //red
                    disposed_channels[count+(2*imageSize)] = ptrR[3*col]; //blue
                    count++;
            }            
    }
    
    //save junctioned image
    cvSaveImage("anaglyph.bmp", anaglyph);
    printf("OK!\n");
    
    //save vector
    printf("Criando paleta de cores... ");
    FILE *fp;
    fp = fopen("pixelData.dat", "wb");
    
    if(!fp){
        printf("Erro!\n\tErro na criacao de arquivo das paletas de cores. Favor verificar\n");
        exit(-1);
    }
    
    fwrite(disposed_channels, sizeof(uchar), 3*imageSize, fp);
    
    fclose(fp);
    free(disposed_channels);    
    
    printf("OK!\n");
    
    //save image in YUV color space
    BGRtoYUV(anaglyph);    
    cvReleaseImage(&anaglyph);
}

/* Prints help message to user */
void printHelp(){
    printf("Chamada do programa: paleta-vetor <arquivo> {-sbs | -ab} {-e | -d}\n");
    printf("\n-sbs:side-by-side ou -ab:above/below");
    printf("\n-e:encode (transformacao anaglifica) ou -d:decode (reversao para par estereo)");
}

int main(int argc, char* argv[]){
    if(argc < 4){
        printHelp();
        exit(-1);  
    }
    
    char *imageType = argv[2]; //-sbs:sid-by-side or -ab:above/below
    
    // ----- Anaglyph conversion
    if(!strcmp(argv[3],"-e")){    
        printf("--- CONVERSAO DE IMAGEM ANAGLIFICA PARA PAR ESTEREO ---\n");
        IplImage *frame, *frameL, *frameR;        
        //load image
        //images are loaded by openCV with color channels in this order: BGR
        frame = cvLoadImage(argv[1], 1);
        //Simple error handling
        if(!frame){
           printf("Erro ao abrir imagem.");
           exit(-1);
        }            
        //some verifications regarding the image
        if(frame->width % 2 != 0){
              printf("Imagem possui largura não divisível por 2. Favor cortar!");
              exit(-1);
        }
            
        int width = frame->width;
        int height = frame->height;      
            
        //new images will have half width of original image
        CvSize size;
        if(!strcmp(imageType, "-sbs")){
            size = cvSize( width/2, height);
        }
        else if(!strcmp(imageType, "-ab")){
            size = cvSize( width, height/2);
        }
        else{
            printHelp();
            exit(-1);                    
        }
        
        //copy image properties
        frameL = cvCreateImage(size, frame->depth, frame->nChannels);
        frameR = cvCreateImage(size, frame->depth, frame->nChannels);
        cvZero(frameL);
        cvZero(frameR);
        
        //divide frames in two
        separateImages(frame, &frameL, &frameR, width, height, imageType);
                
        //create anaglyph and auxiliary image
        createAnaglyph(frameL, frameR);      
                
        cvReleaseImage(&frame);
        cvReleaseImage(&frameL);
        cvReleaseImage(&frameR);
    }
    // ----- Anaglyph reversion
    else if(!strcmp(argv[3],"-d")){
        printf("--- REVERSAO DE IMAGEM ANAGLIFICA PARA PAR ESTEREO ---\n");
        //revertAnaglyph(argv[1], imageType);
        YUVtoBGR(argv[1]);
        //TODO:integrar a reversão de YUV pra RGB com a reversão para par estéreo. Verificar como salvar os dados em uchar shiftado
    }
    else{
        printHelp();
        exit(-1);      
    }
    
    return 0;
}
