#include "stdafx.h"
#include "HiddenLine.h"
#include <algorithm>
#include <list>

void HiddenLine::MakeNCopies(const std::vector<Edge> &edges)
{
    edgeCopies.clear();
    edgeCopies = std::vector<std::vector<Edge>>(edges.size(), edges);
}


IntersectionPoint HiddenLine::ComputeIntersection(float li, const Edge &ej,
                                                  const FVector3 &observer)
{
    //Creating intersection point XJ
    // 
    //ei is outer edge loop
    //  ej is inner edge loop
    // Find the intersection point xj of li and ej,where edge ej is nearer to the observer than the line containing edge ei
    //Should also collect the 
    //create two end points. for ej. aj is start, bj is end. 
    //  if aj is above li, label xj as a left and otherwise as a right endpoint
}


std::vector<Edge> HiddenLine::DetermineUnion(const std::vector<Edge> &edges, const Edge &currentEdge,
                                             const FVector3& observer)
{
    //Assuming that all the endpoints of the input intervals are disjoint
    //1 
    // Sort the endpoints of the intervals in increasing order
    // Prepare a doublly linked list D and relabel them such that x1, x2 ,x3 is the sorted sequence
    //   pred(xi) = xi-1, succ(xi) = xi+1, 2 <= i < = 2n-1,pred(x1) = nil and succ(x2n) = nil
    //2
    // Assign weights wi to xi, 1 <= i <= 2n, 
    // such that if x1 is a left endpoint, then wi =1,
    //  and if xi is a right endpoint , then wi = -1
    //3
    // compute the parallel prefix sum ci = w1 + w1 + ... + wi for all xi 1<= i <= 2n
    //4
    // for all xj, j = 1, 3, ..., 2n-1, do in parrellel
    //          if((xj is a left endpoint and cj > 1) or (xj is a right endpoint and cj >0))
    //          then
    //              remove xj from doubly linked list D
    // end for
    //5
    //  repeat step 4 for all xj, j=2,4, ..., 2n in parrell;
    //6
    //  rank the doubly linked list D, and write the endpoints of the M <= n
    // output intervals parallely into 2M consecutive cells of the global memory
}


std::vector<Edge> HiddenLine::EliminateHiddenLines(std::vector<Edge> &edges, const FVector3 &observer)
{
    //1
    MakeNCopies(edges); 
    for (auto &edgeCopy : edgeCopies)
    {
        //2
        for (int i = 0; i < edgeCopy.size(); i++)
        {
            //2.1
            for (int j = 0; j < edgeCopy.size(); j++) 
            {
                if (i != j)
                {
                    //2.1.1 and 2.1.2
                    float li = 0; //xaxis
                    Edge ej = edgeCopy[j];
                    IntersectionPoint xj= ComputeIntersection(li, ej, observer); 
                    //do something with xj
                }
                //2.2
                // Let xl be a point of li to the left of the leftmost xj, 
                // Let xr be a point of li to the right of the rightmost xj
                // Let xa be the left endpoint of ei
                // Let xb be the right endpoint of ei
                // Label xl and xb as left
                // Label xa and xr as right

                // 2.3 
                // Determine the union of the intervals specifed by the endpoints xl, xa, xb, xr and xj
                // 1 <= j <= n; j!=i

                // 2.4 Insert xa and xb into the list L obtained as a result of step 2.3
                // If insertion of xa fails, then report interval [xa, succ(xa)] as visible segment of ei
                // If insertion of xb fails, then report interval [pred(xb), xb] as visible segment of ei
                // Otherwise [pred(xb),xb] is a hidden interval of ei
                // 
                // 2.5
                // Discard the ements of L left to xa and those right to xb.
                // If two consectutive elements xj and xk, j!=a, k!=b, of L are a left and a right endpoint respectively
                // then [xj, xk] is a visible segment of ei
                // Otherwise if xj is a right, and xk is a left endpoint, then report (xj,xk) as a visible segment of ei





            }
        }
    }
}

