#include"MyAL.h"

#include<iostream>
int main()
{
   // MyAL al;
    std::vector<std::string> vs = MyAL::get_devices();
    std::vector<std::string> vs2 = MyAL::get_capture_devices();
   int audio_choix = 0;
   int capture_choix = 0;
   int k=0;

   //choix haut-parleurs
   std::cout << "devices :" << std::endl;
   for(std::string s : vs)
        std::cout << "\t" << k++ << ") "<< s << std::endl;
    if(vs.size()>1){
        std::cout << "choix : ";
        std::cin >> audio_choix;
    }
    k=0;

    //choix micro (si 1 seul choix => pas de choix)
   std::cout << "capture devices :" << std::endl;
   for(std::string s : vs2)
        std::cout << "\t" << k++ << ") "<< s << std::endl;
    if(vs2.size()>1){
        std::cout << "choix : ";
        std::cin >> capture_choix;
    }

    //enregistrement:
    std::chrono::milliseconds dura(4000);
    std::vector<ALshort> samples;
    //enregistre
    AL_Capture alc(vs[audio_choix],vs2[capture_choix]);//port audio / micro
    alc.start(samples);
    //dors dura milliseconds
    std::this_thread::sleep_for(dura);
    alc.stop();//arret enregistrement
    std::cout << samples.size() << std::endl;
    //on enregistre le son
    alc.save_sound("test.wav");
}
