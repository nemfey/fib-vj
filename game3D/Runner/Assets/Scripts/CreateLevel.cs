using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateLevel : MonoBehaviour
{
    // paths
    public GameObject wallFloorPrefab, wallRampPrefab, wallTurnRightPrefab, wallTurnLeftPrefab;

    // obstacles
    public GameObject wallBarrelsPrefab;

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
        currentX = -60f; // Virtual previous of the first section
        currentZ = -55f; // Virtual previous of the first section
        previousSize = 5; // Random in the future
        nthSection = 0;
        currentChunkY = 0f;

        for (uint i=0; i<7; i++)
        {
            GameObject newSection = createSection();
            sections.Enqueue(newSection);
        }
    }

    // Update is called once per frame
    void Update()
    {
        // cuando se haga un giro del player
            // se destruye una seccion
            // se construye una nueva
            // siempre 5 en pantalla
        // generacion de la seccion
            // primer bloque un suelo
            // ultimo suelo un giro
            // de los bloques del medio
                // se elige uno random para que sea la primera casilla trampa
                // con cierta probablidad las siguientes casillas disponibles tambien seran de la misma trampa
    }

    // Create new section
    GameObject createSection()
    {
        GameObject section;
        section = new GameObject("Section");

        int sectionSize = nthSection < 4 ? 5 : Random.Range(4, 8);

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

        bool bRampPlaced = false;
        // chunk generator obstacle
        int obstacleChunk = Random.Range(1, sectionSize - 1);

        for (uint i = 0; i < sectionSize; i++)
        {
            if (nthSection%2 == 0  && i == 1)
            {
                chunk = (GameObject)Instantiate(wallRampPrefab);
                bRampPlaced = true;
            }
            else if (i == obstacleChunk && nthSection > 2)
            {
                chunk = (GameObject)Instantiate(wallBarrelsPrefab);
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
        }
    }

    public void newSectionProcedure()
    {
        GameObject newSection = createSection();
        sections.Enqueue(newSection);

        Destroy(sections.Dequeue());
    }
}
