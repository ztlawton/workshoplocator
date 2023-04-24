#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"



std::vector<eWorkShopType>
workshopMixFromUser()
{
    std::cout << "Input Workshop Mix\n"
                 "Enter number of each workshop type in order\n"
                 "Agriculture Biotech Electronics Energy HeavyIndustry\n";

    std::vector<eWorkShopType> ret;
    int count;
    for (int k = 0; k < 5; k++)
    {
        std::cin >> count;
        for (int w = 0; w < count; w++)
            ret.push_back((eWorkShopType)k);
    }
    return ret;
}
main()
{
    // get required workshop mix
    std::vector<eWorkShopType> mix = workshopMixFromUser();

    cLayout L;
    L.setWorkshopMix(mix);
    L.calculateLayout();
    std::cout << L.text();
    
    cAsciiArt A;
    A.display( L );

    return 0;
}
