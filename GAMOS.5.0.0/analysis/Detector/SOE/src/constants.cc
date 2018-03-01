//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//

#include "constants.h"

bool doubleEquals(double left, double right, double epsilon)
{
	return (fabs(left - right) < epsilon);
}

void
WriteString(std::ostream& io_os, const std::string& in_string)
{
	// int size = htonl( in_string.length() );
	int size = in_string.length();
	io_os.write( (char*) &size, sizeof(size) );
	io_os.write( &in_string[0], size );
}

void
ReadString(std::istream& in_is, std::string& io_string)
{
   size_t length;
   in_is.read((char*)&length, sizeof(size_t));
   in_is.width(length);
   in_is >> io_string;
}

void bubbleSort( int size, double* x )
{
   int n = size;
   while (n != 0)
   {
      int newn = 0;
      for (int i = 1; i < n; i++)
      {
         if (x[i-1] > x[i])
         {
            double tmp = x[i];
            x[i] = x[i-1];
            x[i-1] = tmp;
            newn = i;
         }
      }
      n = newn;
   }
}

int solveQuadraticEquation(const double& in_aFactor, const double& in_bFactor, const double& in_cFactor
		, double& out_x1, double& out_x2)
{
    double dis = (in_bFactor * in_bFactor) - (4 * in_aFactor * in_cFactor);
    int numSolutions = 0;
    if (dis >= 0)
    {
        out_x1 = ((-1 * in_bFactor) + (sqrt(dis)))/(2 * in_aFactor);
        numSolutions++;

        if (dis > 0)
        {
            out_x2 = ((-1 * in_bFactor) - (sqrt(dis)))/(2 * in_aFactor);
            numSolutions++;
        }
	}
    return numSolutions;
}
