using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FogMovement : MonoBehaviour
{
    public Vector3 playerCoords;
    GameObject player;
    float minY;

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.Find("Player");
        minY = player.GetComponent<Transform>().position.y - 30;
        //playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
    }

    // Update is called once per frame
    void Update()
    {
        playerCoords = player.GetComponent<Transform>().position;

        if (playerCoords.y - 30 < minY)
            minY = playerCoords.y - 30;

        this.transform.position = new Vector3(playerCoords.x, minY, playerCoords.z);
    }
}
