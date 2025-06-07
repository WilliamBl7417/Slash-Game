#pragma once

#include "DrawDebugHelpers.h"
#include "CoreMinimal.h"
#define Draw_Sphere(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location ,25.f ,10 ,FColor::Red ,true);
#define Draw_SphereColor(Location,Color) if(GetWorld()) DrawDebugSphere(GetWorld(),Location ,8.f ,10 ,Color ,false,5.f);
#define Draw_Sphere_SingleFrame(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location ,25.f ,10 ,FColor::Red ,false,-1.f);
#define Debug_Text(Color,Text)if(GEngine){GEngine->AddOnScreenDebugMessage(1, 10.f, Color, Text);}
#define Draw_Line(StartLocation, EndLocation) if (GetWorld())DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define Draw_Line_SingleFrame (StartLocation, EndLocation) if (GetWorld())DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, False, -1.f, 0, 1.f);
#define Draw_Point(Location) if (GetWorld()) DrawDebugPoint(GetWorld(),Location,15.f,FColor::Blue,true);
#define Draw_point_SingleFrame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(),Location,15.f,FColor::Blue,false, -1.f);
#define Draw_SphereC(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location,15.f,12,Color,true);
#define Draw_Box(World,Location,Color) if(World) DrawDebugBox (World,Location,FVector (10.f,10.f,10.f) ,Color,true);


#define Draw_Vector(StarLocation,EndLocation) if (GetWorld())\
{ \
DrawDebugLine(GetWorld(), StarLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
DrawDebugPoint (GetWorld () ,Location ,15.f ,FColor::Blue ,true);\
}


#define Draw_Vector_SingleFrame(StarLocation,EndLocation) if (GetWorld())\
{ \
DrawDebugLine(GetWorld(), StarLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f); \
DrawDebugPoint (GetWorld () ,Location ,15.f ,FColor::Blue ,false);\
}



