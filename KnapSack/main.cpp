#include <bits/stdc++.h>

using namespace std;
vector<vector<bool>> chromosomes;
vector<int> weights;
vector<int> values;
vector<int> fitness;
int n,w;
int maxAnswer=0;
//bool isEnd= false;
vector<bool> generateChromosomes(){
   vector<bool> temp(n);
//   cout<<"vector : "<<chromosomes.size()<<"\n";
    for (int i=0;i<n;i++) {
        bool geneStatus = round(((double) rand() / (RAND_MAX)));
        temp[i] = geneStatus;
//        cout<<"gene Status "<<geneStatus <<"\n";
    }

    return(temp);
}
int calcFitness(vector<bool> temp){// need to put in your consideration the weight of the items to filter the valid chromosomes.
    int fitness=0;
    int collectedWeight=0;
    for(int i=0;i<n;i++){
        fitness+=values[i]*temp[i];
        collectedWeight+=(weights[i]*temp[i]);
    }
    if (collectedWeight>w)
        return -1;
//    else {
//        if (fitness>maxAnswer)
//            maxAnswer=fitness;
//    }
    return fitness;
}

int generatePopulation(){// it should be random number between {(1.5 -- 2) x numberOf Elements} .
   // multiply the number of elements by 1.5 to get the population size.
   int populationSize= 1.5*n;
    for (int i = 0; i < populationSize; ++i) {
        vector<bool> temp (generateChromosomes());
        int generatedFitness=calcFitness(temp);
        if (generatedFitness==-1||generatedFitness==0)
            i--;
        else {
            chromosomes.push_back(temp);
            fitness.push_back(generatedFitness);
        }
    }

//    for (int i = 0; i < chromosomes.size(); ++i) { uncomment to see the chromosomes after generation.
//
//        cout<<"chromosome no = "<<i<<" : ";
//        for (int j = 0; j < n; ++j) {
//            cout<<chromosomes[i][j];
//        }
//        cout<<"  -fitness = " << fitness[i]<<"\n";
//    }

}

pair<int,int> selection(){
    //calculate the cumulative fitness :
    vector<int> cumFitness(fitness.size()+1);
    cumFitness[0]=0;
    for (int i = 0; i < fitness.size(); ++i) {
        cumFitness[i+1]=cumFitness[i]+fitness[i];
//        cout<<"cumFitness= "<<cumFitness[i+1]<<"\n";
    }
    // select the parents :
//    cout<<"cumulative = "<<cumFitness[fitness.size()]<<endl;
    int p1=rand()%cumFitness[fitness.size()];
    int p2=rand()%cumFitness[fitness.size()];
//    cout<<"p1  - p2 "<<p1<<"  "<<p2<<endl;
    int p1Index,p2Index;
    for (int i = 0; i <fitness.size() ;++i) {
        if (cumFitness[i]<p1&& p1<=cumFitness[i+1] &&cumFitness[i]<p2 && p2<=cumFitness[i+1]){
            return selection(); // in case both point in the same range
        }
        else if (cumFitness[i]<p1&& p1<=cumFitness[i+1])
        {
            p1Index=i;
//            cout<<"at p1 "<<fitness[i]<<" "<<p1<<endl;
        }
        else if (cumFitness[i]<p2 && p2<=cumFitness[i+1]){
//            cout<<"at p2 "<<fitness[i]<<" "<<p2<<endl;
            p2Index=i;
        }
    }
//    cout<<p1Index<<" ---- "<<p2Index<<" \n";
//    cout<<"cumulTIVE =";
//    for (int i = 0; i < cumFitness.size(); ++i) {
//        cout<<cumFitness[i]<<" ";
//    }
//    cout<<endl;
    return  make_pair(p1Index,p2Index);// return the specifed parents to make cross over
}
void printVector(vector<bool> v){
//    cout<<"vector ";
//    for (int i = 0; i < n; ++i) {
//        cout<<v[i]<<" ";
//    }
    cout<<"";
}
void recoreectChromosome(int index){
//    cout<<"old fitness = "<<fitness[index]<<"\n";
    while (fitness[index]==-1){// try to keep all genes in the available weight size;
        int flipGene= rand()%(n-1);
        chromosomes[index][flipGene]= !chromosomes[index][flipGene];// flip the gene
        fitness[index]= calcFitness( chromosomes[index] );// recalculate the fitness after flipping the gene;
    }

//    cout<<"newwwww fitness = "<<fitness[index]<<"\n";
}
vector<bool> mutationHelper(vector<bool> parent){
    for (int i = 0; i < n; ++i) {
        bool doMutationForThisGene=round(((double) rand() / (RAND_MAX)));// return 0 or 1
//        cout<<"mutation result "<<doMutationForThisGene<<" ";
        doMutationForThisGene? parent[i]= !parent[i] : parent[i] ;// flip the boolean to do mutation else keep it as it is
    }
//    cout<<"\n";
    return parent;

}
void mutation (int p1,int p2){
//    cout<<"before fitnesss Calc"<<fitness[p1]<<"  "<<fitness[p2]<<"\n";
    chromosomes[p1]= mutationHelper(chromosomes[p1]);
    fitness[p1]=calcFitness(chromosomes[p1]);// recalculate the fitness of the new chromosome
    recoreectChromosome(p1);
    chromosomes[p2] = mutationHelper((chromosomes[p2]));
    fitness[p2]=calcFitness(chromosomes[p2]);
    recoreectChromosome(p2);


//    cout<<"after fitnesss Calc"<<fitness[p1]<<"  "<<fitness[p2]<<"\n";
}
void crossover(){
    srand(time(0));
    bool doCrossOver=round(((double) rand() / (RAND_MAX))); // to check if we have to do crossOver Or Not
    if (!doCrossOver)
        return;
    pair<int,int> parents= selection();// the index of the parents (Chromosomes to be crossedOver)
    int seperationPoint= rand()%(n-1);
//  cout<<"p1 = "<<parents.first<<"   --    "<<"p2 "<<parents.second<<"\n";
//    cout<<"seperation point "<<seperationPoint<<endl;
    vector<bool> tempParent = chromosomes[parents.first];
    printVector(tempParent);
    printVector(chromosomes[parents.second]);
    for (int i = seperationPoint; i < n; ++i) {
        chromosomes[parents.first][i]=chromosomes[parents.second][i];
        chromosomes[parents.second][i]=tempParent[i];
    }
//    cout<<"after cross over \n";
    printVector(chromosomes[parents.first]);
    printVector(chromosomes[parents.second]);
    mutation(parents.first ,parents.second);

//    cout<<"after mutation \n";
    printVector(chromosomes[parents.first]);
    printVector(chromosomes[parents.second]);

}
/*
 * 0.00125126
0.563585
0.193304
0.808741
0.585009
 * */
int iterations=1000;
bool reCalcFitness(int index){
    bool keepGoing= false;// if there is any progress for the max answer then we will continue searching else stop
    if (index<iterations)
        keepGoing=true;
    for (int i = 0; i < chromosomes.size(); ++i) {
        int generatedFitness=fitness[i];

        if (generatedFitness>maxAnswer)// if the new fitness better than the old then keep going chromosomes
        {                              // (repeat the process again -cross over .. etc-)
            maxAnswer=generatedFitness;
            keepGoing= true;
        }
//        cout<<"new Fitness = "<<generatedFitness<<"\n";
    }
    return keepGoing;
}
int main() {
//    cout<<"enter the number of items :\n";
int t ;
cin>>t;int x=1;
    while (t--){
        chromosomes.clear();
        fitness.clear();
        maxAnswer=0;
        srand(time(0));
        cin>>n>>w;
        weights.resize(n);
        values.resize(n);
        for (int i = 0; i < n; ++i) {
            cin>>weights[i]>>values[i];
        }
        generatePopulation();
        crossover();

int index=0;
        while (reCalcFitness(index++)){
//            cout<<x <<") final Answer V1= "<<maxAnswer<<"\n";
//            cout<<"\nsecond Iteration\n";
            crossover();
        }

        cout<<x++ <<") final Answer = "<<maxAnswer<<"\n";
    }



   // generateChromosomes(151);

//    selection();



return 0;
}
//how to make it repeat the cross over there is something in the condition of reCalacFitness
