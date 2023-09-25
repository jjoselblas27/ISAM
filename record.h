#ifndef RECORD_HPP
#define RECORD_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#pragma pack(push, 1)
struct AudioFeatures
{
public:
    char isrc[13]{'\0'};
    double acousticness;
    double danceability;
    int duration_ms;
    double energy;
    double instrumentalness;
    int key;
    double liveness;
    double loudness;    
    int mode;
    double speechiness;
    double tempo;
    int time_signature;
    double valence;

    // Constructor por defecto
    AudioFeatures() :  acousticness(0.0), danceability(0.0), duration_ms(0), energy(0.0), instrumentalness(0.0),
            key(-1), liveness(0.0), loudness(0.0), mode(0), speechiness(0.0), tempo(0.0),
            time_signature(0), valence(0.0) {}

    // Constructor con parámetros
    AudioFeatures(string isrc,double ac, double da, int du, double en, double ins, int k, double li, double lou,
                    int m, double sp, double te, int ts, double va)
            : acousticness(ac), danceability(da), duration_ms(du), energy(en), instrumentalness(ins),
            key(k), liveness(li), loudness(lou), mode(m), speechiness(sp), tempo(te),
            time_signature(ts), valence(va) {

                std::copy(isrc.begin(), isrc.end(), this->isrc);

            }
    
    std::string to_string() const {
        std::ostringstream oss;
        oss << "(" << isrc << ", " << acousticness << ", " << danceability << ", " << duration_ms << ", " << energy
            << ", " << instrumentalness << ", " << key << ", " << liveness << ", " << loudness
            << ", " << mode << ", " << speechiness << ", " << tempo << ", " << time_signature
            << ", " << valence << ")";
        return oss.str();
    }
};


#pragma pack(pop)

#endif // RECORD_HPP