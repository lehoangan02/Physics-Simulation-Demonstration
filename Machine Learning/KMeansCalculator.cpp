//
// Created by Le Hoang An on 30/11/24.
//

#include "KMeansCalculator.h"
#include "raymath.h"
KMeansCalculator* KMeansCalculator::getKMeansCalculator() {
    static KMeansCalculator MyKMeansCalculator;
    return &MyKMeansCalculator;
}
std::vector<Vector2> KMeansCalculator::operator()(const std::vector<Vector2>& Data, const int &K)
{
    int NumData = static_cast<int>(Data.size());
    std::vector<Vector2> TempCentroids(K);
    std::vector<Vector2> Centroids(K);
    float MinCost = 1e9;
    for (int i = 0; i < m_Attempt; ++i)
    {
        for (int j = 0; j < K; ++j)
        {
            TempCentroids[j] = Data[static_cast<int>(std::rand() % NumData)];
//            std::sort(TempCentroids.begin(), TempCentroids.end(), [](Vector2 A, Vector2 B) {
//                return A.x < B.x;
//            });
        }
        std::vector<int> Assignment(NumData);
        for (int j = 0; j < m_Limit; ++j) {
            for (int k = 0; k < NumData; ++k)
            {
                float MinDistance = 1e9;
                for (int l = 0; l < K; ++l)
                {
                    float Distance = calculateSquareDistance(Data[k], TempCentroids[l]);
                    if (Distance < MinDistance)
                    {
                        MinDistance = Distance;
                        Assignment[k] = l;
                    }
                }
                for (int l = 0; l < K; ++l)
                {
                    Vector2 Average = {0, 0};
                    int Count = 0;
                    for (int n = 0; n < NumData; ++n)
                    {
                        if (Assignment[n] == l)
                        {
                            Average = Vector2Add(Average, Data[n]);
                            Count++;
                        }
                    }
                    if (Count > 0) {
                        TempCentroids[l] = Vector2Scale(Average, 1.0f / static_cast<float>(Count));
                    }
                }

            }
        }
        float Cost = calculateCost(Data, TempCentroids, Assignment);
        if (Cost < MinCost)
        {
            MinCost = Cost;
            Centroids = TempCentroids;
            m_Assignment = Assignment;
        }

    }
    return Centroids;
}
float KMeansCalculator::calculateSquareDistance(Vector2 A, Vector2 B)
{
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}
float KMeansCalculator::calculateCost(const std::vector<Vector2> &Data, const std::vector<Vector2> &Centroids, const std::vector<int> &Assignment)
{
    float Cost = 0;
    for (int i = 0; i < Data.size(); ++i) {
        Cost += calculateSquareDistance(Data[i], Centroids[Assignment[i]]);
    }
    return Cost;
}
std::vector<int> KMeansCalculator::getAssignment()
{
    return m_Assignment;
}