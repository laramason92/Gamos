#include "SealTest/SealTest.h" 
#include "SealBase/StringOps.h"
#include "SealBase/Regexp.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::StringOps02_03(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    StringList l;
    l.push_back ("abc");
    l.push_back ("123");
    seal_test::out << StringOps::join (StringOps::grep (l, Regexp("\\d+")), ":")
	      << "\n";
    return 0;
}