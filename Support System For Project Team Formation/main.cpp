/**
 * authors :Mahmoud Ashraf { 20180250 } , Fady Emad { 20180193 }.
 */
#include <iostream>
#include <vector>

using namespace std;

double calculateSlope(int x1,int y1,int x2,int y2){
    return (y2-y1)/double(x2-x1);
}
double calculate_C(int y,double m,int x){
     return double (y-m*x);
}

void Fuzzification(double point  , vector<vector<int>> values , double y_values[]){

    for (int i = 0; i < values.size() ;++i) {
        for (int j = 0; j < values.size()-1; ++j) {
            if (values[i][j]<=point && point<values[i][j+1])// it intersect with this line
            {
//                cout<<project_funding[i]<<" : "<<fundingValues[i][j]<<endl;
                int y1,y2, x1= values[i][j],x2= values[i][j+1];
                if (j==0){
                    y1=0;
                    y2=1;
                }else if (j==1){
                    y1=1;
                    y2=values.size()==3? 0 : 1; // triangle differs from trapezoid
                }else {
                    y1=1;
                    y2=0;
                }
                double slope= calculateSlope(x1,y1 ,x2,y2);
//                cout<<slope<<endl;
               double c= calculate_C(y1 ,slope,x1 );
//               cout<<"c = "<<c<<endl;
               double y=slope*point+c;
//               cout<<"y = "<<y<<endl;
               y_values[i]= y ;
               break;
            }
        }
        //cout<<"whaaat\n";
    }
//    cout<<"at size "<<values.size()<<" \n";
//    for(int i=0;i<values.size();i++)
//        cout<<i<<") "<<y_values[i]<< " ";
//    cout<<"\nend\n";
}
void Inference(double fund[],double experience[],double InferenceAnswer[]){
    //// {"very low" , "low" , "medium","high"};
    //// {"beginner" , "intermediate" , "expert"};
    // If project_funding is high or team_experience_level is expert then risk is low.
    InferenceAnswer[0]= max(fund[3],experience[2]);
    //If project_funding is medium and team_experience_level is intermediate or
    // team_experience_level is beginner then risk is normal.
    InferenceAnswer[1]=max( min(fund[2],experience[1]) ,experience[0] );

    // If project_funding is very low then risk is high. -- R1
    //If project_funding is low and team_experience_level is beginner then risk is high. -- R2
    //// InferenceAnswer[High] = (R1 or R2 )
    InferenceAnswer[2]=max(fund[0],min(fund[1],experience[0]));
}
double defuzzication(double rulesAnswer[] ,vector<vector<int>> riskValues){
    double cValues[3]={0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cValues[i]+=riskValues[i][j];
        }
        cValues[i]=cValues[i]/3;
    }
    /*   z* = (Σ𝝁(𝒛 ). 𝒛) / Σ𝝁(𝒛 )  */
    double Z= ( rulesAnswer[0]*cValues[0] + rulesAnswer[1]*cValues[1]+rulesAnswer[2]*cValues[2] )/(rulesAnswer[0]+rulesAnswer[1]+rulesAnswer[2]);
//    cout<<"\nZ = "<<Z<<endl;
    return  Z;
}
int main() {
//    string project_funding []= {"very low", "low" , "medium","high"};
//    string  team_experience_level[]={"beginner","intermediate","expert"};

    vector<vector<int>> fundingValues= {{0,0,10,30},{10,30,40,60},{40,60,70,90},{70,90,100,100}};
    vector<vector<int>> experienceValues ={{0,15,30},{15,30,45},{30,60,60}};
    string risk[]={"low", "normal", "high"};
    vector<vector<int>> riskValues ={ {0,25,50},{25,50,75},{50,100,100} };

    int variables ;
    double projectFund,experienceLvl;
//    cout<<"enter the number of variables : ";
//    cin>>variables;
    cout<<"enter the Project Fund : ";
    cin>>projectFund;
    cout<<"enter the Experience Level : ";
    cin>>experienceLvl;
    ////step1 : Fuzzification
    double fundMemberShip[4]={0}, experienceMemberShip[3]={0} , rulesAnswer[3]={0};
    Fuzzification(projectFund,fundingValues,fundMemberShip);
    Fuzzification(experienceLvl,experienceValues,experienceMemberShip);

    ////step2 : Inference
    Inference(fundMemberShip,experienceMemberShip,rulesAnswer);

    ////step3 : defuzzification
    double Z=defuzzication(rulesAnswer,riskValues);
    double riskMemberShip[3]={0};
    Fuzzification(Z,riskValues,riskMemberShip);

    int zIndex;
    double maxResult=0;
    for (int i = 0; i< 3; ++i) {
        if (riskMemberShip[i]>maxResult)
        {
            maxResult=riskMemberShip[i];
            zIndex=i;
        }
    }
//    cout<<zIndex<<" zz\n";
    cout<<"\nPredicted Value (Risk) = {"<<Z<<"} Risk will be "<<risk[zIndex]<<".\n";
    return 0;
}
