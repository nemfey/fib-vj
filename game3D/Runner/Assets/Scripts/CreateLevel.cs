using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateLevel : MonoBehaviour
{
    public GameObject wallFloorPrefab, wallTurnRightPrefab, wallTurnLeftPrefab;

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
            nthSection++;
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

        System.Random random = new System.Random();
        int sectionSize = nthSection < 4 ? 5 : random.Next(4, 8);
        //int sectionSize = random.Next(4, 8);

        previousSize = sectionSize;
        createSectionChunks(section, sectionSize);

        if (nthSection % 2 == 0)
        {
            lastX += 5.0f;
            lastZ += previousSize * 5.0f;
            section.transform.Translate(lastX, 0.0f, lastZ);
            section.transform.Rotate(0.0f, 0.0f, 0.0f);
        }
        else
        {
            lastX += (previousSize - 1) * 5.0f;
            section.transform.Translate(lastX, 0.0f, lastZ);
            section.transform.Rotate(0.0f, 90.0f, 0.0f);
        }

        previousSize = sectionSize;
        section.transform.parent = transform;

        return section;
    }

    void createSectionChunks(GameObject section, int sectionSize)
    {
        GameObject chunk = null;

        for (uint i = 0; i < sectionSize; i++)
        {
            if (i != 0)
                chunk = (GameObject)Instantiate(wallFloorPrefab);
            else if (i == 0)
            {
                if (nthSection % 2 == 0)
                    chunk = (GameObject)Instantiate(wallTurnRightPrefab);
                else
                    chunk = (GameObject)Instantiate(wallTurnLeftPrefab);
            }

            chunk.transform.Translate(0.0f + i * 5.0f, 0.0f, 0.0f); // CAMIBAR EJES DE COORDS(?)
            chunk.transform.parent = section.transform;
        }
    }

    public void newSectionProcedure()
    {
        nthSection++;
        GameObject newSection = createSection();
        sections.Enqueue(newSection);

        Destroy(sections.Dequeue());
    }
}
