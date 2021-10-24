// #include "trace.h" sert
// //Ramer-Douglas-Peucker

// void splitDots(Dot dots[], Dot out[], size_t start, size_t end, size_t
// outLen){
//     for (size_t i = 0; i < outLen && start <= end; i++, start++){
//         out[i] = dots[start];
//     }
// }

// double perpendicularDistance(Dot dot, Line l){
//     double dx = abs(l.end.X - l.start.X);
//     double dy = abs(l.end.Y - l.start.Y);

//     double normalize = sqrt(dx*dx + dy*dy);
//     if (normalize){
//         dx /= normalize;
//         dy /= normalize;
//     }

//     double pvX = dot.X - l.start.X, pvY = dot.Y - l.start.Y;

//     //Projection of pv into normalized direction
//     double pvDot = dx * pvX + dy * pvY;

//     //Scale of the line direction vectors
//     double dsX = pvDot * dx, dsY = pvDot * dy;

//     //Subract the scale from the pv
//     double distX = pvX - dsX, distY = pvY - dsY;
//     return sqrt(distX*distX + distY*distY);
// }

// void RDP(Dot dots[], size_t lenDots, double epsilon, D  ot output[], size_t
// outLen){
//     if (lenDots < 2)
//         errx(1,"Not enough dot to simplify");

//     double dMax = 0.0;
//     size_t index = 0, end = lenDots - 1;
//     for (size_t i = 1; i < end; i++){
//         Line l;
//         l.start = dots[0], l.end = dots[end];
//         double d = perpendicularDistance(dots[i], l);
// 		if (d > dMax){
// 			index = i;
// 			dMax = d;
// 		}
//     }

//     // If max distance is greater than epsilon, recursively simplify
//     if (dMax > epsilon){
//         size_t len1 = lenDots - index - 1;
//         size_t len2 = lenDots - index;
//         Dot recRes1[len1];
//         Dot recRes2[len2];
//         splitDots(dots, recRes1, 0, index, len1);
//         splitDots(dots, recRes2, index, end, len2);
//         RDP(recRes1, len1, epsilon);
//         RDP(recRes2, len2, epsilon);
//     }else{
//         Dot resList[2] = {dots[1], dots[end]};
//     }
// }
