using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateLevel : MonoBehaviour
{
    // paths
    public GameObject wallFloorPrefab, wallTurnRightPrefab, wallTurnLeftPrefab;

    // obstacles
    public GameObject wallBarrelsPrefab;

    Queue<GameObject> sections = new Queue<GameObject>();

    float lastX;
    float lastZ;
    float previousSize;
    int nthSection;

    // Start is called before the first frame update
    void Start()
    {
        lastX = -60.0f; // Virtual previous of the first section
        lastZ = -55.0f; // Virtual previous of the first section
        previousSize = 5; // Random in the future
        nthSection = 0;

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
            lastX += previousSize * 5.0f;
            lastZ += 5.0f;
            section.transform.Translate(lastX, 0.0f, lastZ);
            section.transform.Rotate(0.0f, 0.0f, 0.0f);
        }
        else
        {
            lastZ += (previousSize - 1) * 5.0f;
            section.transform.Translate(lastX, 0.0f, lastZ);
            section.transform.Rotate(0.0f, 90.0f, 0.0f);
        }

        previousSize = sectionSize;
        section.transform.parent = transform;
        nthSection++;

        return section;
    }

    void createSectionChunks(GameObject section, int sectionSize)
    {
        GameObject chunk = null;

        // chunk generator obstacle
        int obstacleChunk = Random.Range(1, sectionSize - 1);

        for (uint i = 0; i < sectionSize; i++)
        {
            if (i == obstacleChunk && nthSection > 2)
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

            chunk.transform.Translate(0.0f, 0.0f, 0.0f + i * 5.0f);
            chunk.transform.parent = section.transform;
        }
    }

    public void newSectionProcedure()
    {
        GameObject newSection = createSection();
        sections.Enqueue(newSection);

        Destroy(sections.Dequeue());
    }
}
