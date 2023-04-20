using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateLevel : MonoBehaviour
{
    public GameObject wallFloorPrefab, wallTurnRightPrefab, wallTurnLeftPrefab;

    Queue<GameObject> sections = new Queue<GameObject>();

    // Start is called before the first frame update
    void Start()
    {
        GameObject section;

        float lastX = -60.0f; // Virtual previous of the first section
        float lastZ = -60.0f; // Virtual previous of the first section
        float previousSize = 5; // Random in the future

        for (uint i=0; i<7; i++)
        {
            section = new GameObject("Section");
            int sectionSize = 5; // RANDOM IN THE FUTURE
            createNewSection(section, sectionSize);

            if (i % 2 != 0)
            {
                lastX += 10.0f;
                lastZ += previousSize * 5.0f;
                section.transform.Translate(lastX, 0.0f, lastZ);
                section.transform.Rotate(0.0f, 0.0f, 0.0f);
            }
            else
            {
                lastX += (previousSize-1) * 5.0f;
                lastZ += 5.0f;
                section.transform.Translate(lastX, 0.0f, lastZ);
                section.transform.Rotate(0.0f, 90.0f, 0.0f);

            }

            // ACTUALIZAR PREVOUS SIZE
            previousSize = sectionSize;
            section.transform.parent = transform;
            sections.Enqueue(section);
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
    void createNewSection(GameObject section, int sectionSize)
    {
        GameObject chunk;

        for (uint i = 0; i < sectionSize; i++)
        {
            chunk = (GameObject)Instantiate(wallFloorPrefab);
            chunk.transform.Translate(0.0f + i * 5.0f, 0.0f, 0.0f); // CAMIBAR EJES DE COORDS(?)
            chunk.transform.parent = section.transform;
        }
    }

    void newSectionProcedure()
    {
        // create a new section and destroy the first one of the queue
    }
}
