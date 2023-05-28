using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateLevel : MonoBehaviour
{
    // paths
    public GameObject wallFloorPrefab, wallRampPrefab, wallTurnRightPrefab, wallTurnLeftPrefab;

    // obstacles
    public GameObject wallBarrelsPrefab, evilCoinPrefab, barrelPrefab, bindweedPrefab, spikeTrapPrefab;
    GameObject[] obstacles = new GameObject[5];

    // decoration
    public GameObject tower1Prefab, tower2Prefab, tower3Prefab, tower4Prefab, tower5Prefab;
    GameObject[] decorations = new GameObject[5];

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
        initializeDecorations();

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
        //int obstacleId = Random.Range(0, obstacles.Length+1); // +1 because probabiliy of not putting any trap
        int obstacleId = Random.Range(0, obstacles.Length);
        HashSet<int> obstacleChunks = selectObstacleChunks(sectionSize, obstacleId);

        // Select if coin and coin chunk
        GameObject coin = (Random.Range(0, 4) == 1) ? (GameObject)Instantiate(coinPrefab) : null;
        int coinChunk = Random.Range(0, sectionSize - 1);
        float currentCoinY = -1f;

        // Select void chunk
        HashSet<int> voidChunks = new HashSet<int>();
        if (obstacleId == 1 || obstacleId == 2 || obstacleId == obstacles.Length)
        {
            voidChunks = selectVoidChunks(sectionSize);
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
            if (obstacleChunks.Contains(i) && sectionSize > 3)
            {
                //if (obstacleId < obstacles.Length)
                //{
                if (obstacles[obstacleId] == evilCoinPrefab)
                {
                    GameObject evilCoin = (GameObject)Instantiate(evilCoinPrefab);
                    float coinY = (Random.Range(0, 2) == 0) ? 20f : 24f;
                    currentCoinY = (currentCoinY == -1f) ? coinY : currentCoinY;
                    //float coinY = (Random.Range(0, 2) == 0) ? 20f : 24f;
                    Vector3 pos = new Vector3(-2.5f, currentChunkY + currentCoinY, i * 5f);
                    placeObstacle(section, evilCoin, pos);
                    chunk = (GameObject)Instantiate(wallFloorPrefab);
                }
                else if (obstacles[obstacleId] == barrelPrefab)
                {
                    GameObject barrel = (GameObject)Instantiate(barrelPrefab);
                    barrel.transform.Rotate(0f, -90f, 0f);
                    Vector3 pos = new Vector3(-1.25f, currentChunkY + 25f, (sectionSize + 2) * 5f);
                    placeObstacle(section, barrel, pos);
                    barrel.SetActive(false);
                    chunk = (GameObject)Instantiate(wallFloorPrefab);
                }
                else
                {
                    chunk = (GameObject)Instantiate(obstacles[obstacleId]);
                }
                //chunk = (GameObject)Instantiate(obstacles[obstacleId]);
                //}
                //else
                //{
                //    chunk = new GameObject("Void");
                //}
            }
            else if (i == sectionSize - 1)
            {
                if (nthSection % 2 == 0)
                    chunk = (GameObject)Instantiate(wallTurnRightPrefab);
                else
                    chunk = (GameObject)Instantiate(wallTurnLeftPrefab);
            }
            else if (voidChunks.Contains(i) && sectionSize > 3)
            {
                chunk = new GameObject("Void");
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
            
            if (coin != null && coinChunk == i && obstacleId != 1)
            {
                float coinY = (Random.Range(0, 2) == 0) ? 20f : 24f;
                currentCoinY = (currentCoinY == -1f) ? coinY : currentCoinY;
                Vector3 pos = new Vector3(-2.5f, currentChunkY + currentCoinY, i * 5f);
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
        int sectionSize = Random.Range(2, 7);
        GameObject newSection = createSection(sectionSize);
        sections.Enqueue(newSection);

        if (nthSection > 9)
        {
            Destroy(sections.Dequeue());
        }


        if (Random.Range(0, 3) == 0)
        {
            Vector3 sectionPosition = newSection.transform.position;
            addDecoration(sectionPosition, sectionSize);
        }
    }

    private void addDecoration(Vector3 sectionPosition, int sectionSize)
    {
        int decorationId = Random.Range(0, decorations.Length);
        GameObject decoration = (GameObject)Instantiate(decorations[decorationId]);

        if (nthSection % 2 == 0)
        {
            Vector3 pos = new Vector3(sectionPosition.x + sectionSize * 5f, sectionPosition.y-10f, sectionPosition.z - 40f);
            decoration.transform.position = pos;
        }
        else
        {
            Vector3 pos = new Vector3(sectionPosition.x - 40f, sectionPosition.y-10f, sectionPosition.z + sectionSize * 5f);
            decoration.transform.position = pos;
        }
    }

    private void initializeObstacles()
    {
        obstacles[0] = wallBarrelsPrefab;
        obstacles[1] = evilCoinPrefab;
        obstacles[2] = barrelPrefab;
        obstacles[3] = bindweedPrefab;
        obstacles[4] = spikeTrapPrefab;

    }

    private void initializeDecorations()
    {
        decorations[0] = tower1Prefab;
        decorations[1] = tower2Prefab;
        decorations[2] = tower3Prefab;
        decorations[3] = tower4Prefab;
        decorations[4] = tower5Prefab;
    }

    private HashSet<int> selectObstacleChunks(int sectionSize, int obstacleId)
    {
        HashSet<int> obstacleChunks = new HashSet<int>();

        // Add init obstacle
        int firstObstacleChunk = Random.Range(1, sectionSize - 1);
        obstacleChunks.Add(firstObstacleChunk);

        if (obstacleId != 2)
        {
            int extraChunks = Random.Range(0, 2);
            if (extraChunks == 1)
            {
                int nextChunk = firstObstacleChunk + (Random.Range(0, 2) + 1);
                if (obstacleId == 1 && (firstObstacleChunk + 1) < sectionSize - 1) // EvilCoins must be grouped
                {
                    nextChunk = firstObstacleChunk + 1;
                    obstacleChunks.Add(nextChunk);
                }
                else if (nextChunk < sectionSize - 1)
                {
                    obstacleChunks.Add(nextChunk);
                }
            }
        }

        return obstacleChunks;
    }

    private HashSet<int> selectVoidChunks(int sectionSize)
    {
        HashSet<int> voidChunks = new HashSet<int>();

        // Add init obstacle
        int firstVoidChunk = Random.Range(1, sectionSize - 1);
        voidChunks.Add(firstVoidChunk);

        int extraChunks = Random.Range(0, 2);
        if (extraChunks == 1)
        {
            int nextChunk = firstVoidChunk + (Random.Range(0, 2) + 1);
            if (nextChunk < sectionSize - 1)
            {
                voidChunks.Add(nextChunk);
            }
        }
        return voidChunks;
    }
}   