using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateLevel : MonoBehaviour
{
    // paths
    public GameObject wallFloorPrefab, wallRampPrefab, wallTurnRightPrefab, wallTurnLeftPrefab;

    // obstacles
    public GameObject wallBarrelsPrefab, evilCoinPrefab, barrelPrefab;
    GameObject[] obstacles = new GameObject[3];

    // Coin
    public GameObject coinPrefab;

    Queue<GameObject> sections = new Queue<GameObject>();

    // section parameters
    float currentX;
    float currentZ;
    float previousSize;
    int nthSection;

    // chunk parameters
    float currentChunkY;

    // Start is called before the first frame update
    void Start()
    {
        currentX = 15f;    // Virtual previous of the first section
        currentZ = -5f;    // Virtual previous of the first section
        previousSize = 5;
        nthSection = -1;
        currentChunkY = 0f;

        initializeObstacles();

        for (uint i=0; i<3; i++)
        {
            GameObject newSection = createSection(5);
            sections.Enqueue(newSection);
        }
    }

    // Update is called once per frame
    void Update() { }

    // Create new section
    GameObject createSection(int sectionSize)
    {
        GameObject section;
        section = new GameObject("Section");

        createSectionChunks(section, sectionSize);

        if (nthSection % 2 == 0)
        {
            currentX += previousSize * 5f;
            currentZ += 5f;
            section.transform.Translate(currentX, 0f, currentZ);
            section.transform.Rotate(0f, 0f, 0f);
        }
        else
        {
            currentZ += (previousSize - 1) * 5f;
            section.transform.Translate(currentX, 0f, currentZ);
            section.transform.Rotate(0f, 90f, 0f);
        }

        previousSize = sectionSize;
        section.transform.parent = transform;
        nthSection++;

        return section;
    }

    void createSectionChunks(GameObject section, int sectionSize)
    {
        GameObject chunk = null;

        // Select obstacle chunk
        int obstacleId = Random.Range(0, obstacles.Length+1);
        HashSet<int> obstacleChunks = selectObstacleChunks(sectionSize);

        // Select if coin and coin chunk
        GameObject coin = null;
        int coinChunk = -1;
        if (Random.Range(0, 4) == 1)
        {
            coin = (GameObject)Instantiate(coinPrefab);
            coinChunk = Random.Range(0, sectionSize-1);
        }

        bool bRampPlaced = false;

        for (int i = 0; i < sectionSize; i++)
        {
            /*
            if (nthSection%2 == 0  && i == 1 && i != sectionSize-1)
            {
                chunk = (GameObject)Instantiate(wallRampPrefab);
                bRampPlaced = true;
            }
            */
            //else if (i == obstacleChunk && nthSection > 2 && sectionSize > 3)
            if (obstacleChunks.Contains(i) && nthSection > 2 && sectionSize > 3)
            {
               if (obstacleId < obstacles.Length)
                {
                    if (obstacles[obstacleId] == evilCoinPrefab)
                    {
                        GameObject evilCoin = (GameObject)Instantiate(evilCoinPrefab);
                        Vector3 pos = new Vector3(-2.5f, currentChunkY + 20f, i * 5f);
                        placeObstacle(section, evilCoin, pos);
                        chunk = (GameObject)Instantiate(wallFloorPrefab);
                    }
                    else if (obstacles[obstacleId] == barrelPrefab)
                    {
                        Debug.Log("BARREL PLACED!");
                        GameObject barrel = (GameObject)Instantiate(barrelPrefab);
                        barrel.transform.Rotate(0f, -90f, 0f);
                        Vector3 pos = new Vector3(-1.25f, currentChunkY + 25f, (sectionSize+2) * 5f);
                        placeObstacle(section, barrel, pos);
                        barrel.SetActive(false);
                        chunk = (GameObject)Instantiate(wallFloorPrefab);
                    }
                    else
                    {
                        chunk = (GameObject)Instantiate(obstacles[obstacleId]);
                    }
                    //chunk = (GameObject)Instantiate(obstacles[obstacleId]);
                }
                else
                {
                    chunk = new GameObject("Void");
                }
            }
            else if (i == sectionSize - 1)
            {
                if (nthSection % 2 == 0)
                    chunk = (GameObject)Instantiate(wallTurnRightPrefab);
                else
                    chunk = (GameObject)Instantiate(wallTurnLeftPrefab);
            }
            else
            {
                chunk = (GameObject)Instantiate(wallFloorPrefab);
            }

            chunk.transform.Translate(0f, currentChunkY, 0f + i * 5f);
            chunk.transform.parent = section.transform;

            if (bRampPlaced)
            {
                currentChunkY -= 3f;
                bRampPlaced = false;
            }
            
            if (coin != null && coinChunk == i)
            {
                Vector3 pos = new Vector3(-2.5f, currentChunkY + 20f, i * 5f);
                placeObstacle(section, coin, pos);
            }
            
        }
    }

    public void placeObstacle(GameObject section, GameObject gObj, Vector3 pos)
    {
        gObj.transform.position = pos;
        gObj.transform.parent = section.transform;
    }

    public void newSectionProcedure()
    {
        int sectionSize = Random.Range(2, 8);
        GameObject newSection = createSection(sectionSize);
        sections.Enqueue(newSection);

        if (nthSection > 9)
        {
            Destroy(sections.Dequeue());
        }
    }

    private void initializeObstacles()
    {
        //obstacles[0] = wallBarrelsPrefab;
        //obstacles[1] = evilCoinPrefab;
        //obstacles[2] = barrelPrefab;
        obstacles[0] = barrelPrefab;
        obstacles[1] = barrelPrefab;
        obstacles[2] = barrelPrefab;
    }

    private HashSet<int> selectObstacleChunks(int sectionSize)
    {
        HashSet<int> obstacleChunks = new HashSet<int>();

        // Add init obstacle
        int firstObstacleChunk = Random.Range(1, sectionSize - 1);
        obstacleChunks.Add(firstObstacleChunk);

        int extraChunks = Random.Range(0, 3);
        for (int i = 0; i < extraChunks; i++)
        {
            int nextChunk = firstObstacleChunk + i + 1;
            if (nextChunk < sectionSize-1)
                obstacleChunks.Add(nextChunk);
        }


        return obstacleChunks;
    }
}   