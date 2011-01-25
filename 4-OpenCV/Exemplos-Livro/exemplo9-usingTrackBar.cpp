#include <cv.h>
#include <highgui.h>

//We make this value global so everyone can see it.
int g_switch_value = 0;

//This will be the callback we give to the trackbar.
void switch_callback(int position){
     if(position == 0){
                 //switch_off_function();
     } else{
            //switch_on_function();
     }
}

int main(int argc, char* argv[]){
    //name the main window
    cvNamedWindow("Demo window");
    
    //create the trackbar. We give it a name, and tell it the name of the
    //parent window
    cvCreateTrackbar(
                     "Switch",
                     "Demo window",
                     &g_switch_value,
                     1,
                     switch_callback
    );
    
    //This will just cause openCV to idle until someone hits the "Escape" key.
    while(1){
             if(cvWaitKey(15) == 27) break;
    }
}
