/*
 * TomoFCut.C
 *
 *  Created on: Mar 16, 2023
 *      Author: gianf
 */

Double_t TomoFCut(Double_t *x, Double_t *p) {

    Double_t val=0.0;

    if (x[0]>p[2]) {
        val=p[0] + p[1]*x[0];
    } else if (x[0]>p[3]) {
        val = p[0]+p[1]*(p[2]-2.0*(p[2]-p[3])) + 2.0*p[1]*TMath::Sqrt(p[2]-p[3])*TMath::Sqrt(x[0]-p[3]);
        if (val<0) { val=0;}
    }
    return val;
}


