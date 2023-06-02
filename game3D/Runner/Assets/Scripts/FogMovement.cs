using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FogMovement : MonoBehaviour
{
    Vector3 playerCoords;
    GameObject player;
    float offsetY, minY;

    public GameObject fogPrefab;
    GameObject farFog, nearFog;

    // Start is called before the first frame update
    void Start()
    {
        offsetY = 15f;
        player = GameObject.Find("Player");
        playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
        minY = playerCoords.y - offsetY;

        farFog = (GameObject)Instantiate(fogPrefab);
        farFog.transform.Translate(playerCoords.x - 200f, minY, playerCoords.z - 200f);
        farFog.transform.parent = transform;

        nearFog = (GameObject)Instantiate(fogPrefab);
        nearFog.transform.Translate(playerCoords.x, minY, playerCoords.z);
        nearFog.transform.parent = transform;
    }

    // Update is called once per frame
    void Update()
    {
        playerCoords = player.GetComponent<Transform>().position;

        if (playerCoords.y - offsetY < minY)
            minY = playerCoords.y - offsetY;

        float distance = Vector3.Distance(playerCoords, farFog.transform.position);
        if (distance > 400f)
        {
            Destroy(farFog);
            farFog = nearFog;

            nearFog = (GameObject)Instantiate(fogPrefab);
            nearFog.transform.Translate(playerCoords.x + 100f, minY, playerCoords.z + 100f);
            nearFog.transform.parent = transform;
        }

        //this.transform.position = new Vector3(playerCoords.x, minY, playerCoords.z);
    }
}
