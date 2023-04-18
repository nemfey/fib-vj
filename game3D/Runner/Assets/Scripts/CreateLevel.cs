using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateLevel : MonoBehaviour
{
    public GameObject wallFloorPrefab, wallTurnRightPrefab, wallTurnLeftPrefab;

    // Start is called before the first frame update
    void Start()
    {
        GameObject section;

        section = new GameObject("Section");
        createNewSection(section);
        section.transform.parent = transform;
        /*
        for (uint i=0; i<5; i++)
        {
            section = new GameObject("Section");
            createNewSection(section);
            section.transform.parent = transform;
        }
        */
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    // Create new section
    void createNewSection(GameObject section)
    {
        GameObject chunk;
        int n = 5; // RANDOM IN THE FUTURE
        for (uint i = 0; i < n; i++)
        {
            chunk = (GameObject)Instantiate(wallFloorPrefab);
            chunk.transform.Translate(0.0f + i * 5.0f, 0.0f, 0.0f); // CAMIBAR EJES DE COORDS(?)
            chunk.transform.parent = section.transform;
        }
    }
}
