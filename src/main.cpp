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
   std::cout << "devices :" << std::endl;
   for(std::string s : vs)
        std::cout << "\t" << k++ << ") "<< s << std::endl;
    if(vs.size()>1){
        std::cout << "choix : ";
        std::cin >> audio_choix;
    }
    k=0;
   std::cout << "capture devices :" << std::endl;
   for(std::string s : vs2)
        std::cout << "\t" << k++ << ") "<< s << std::endl;
    if(vs2.size()>1){
        std::cout << "choix : ";
        std::cin >> capture_choix;
    }
    std::chrono::milliseconds dura(4000);
    std::vector<ALshort> samples;
    AL_Capture alc(vs[audio_choix],vs2[capture_choix]);
    alc.start(samples);
    std::this_thread::sleep_for(dura);
    alc.stop();
    std::cout << samples.size() << std::endl;
    alc.save_sound("test.wav");
}
