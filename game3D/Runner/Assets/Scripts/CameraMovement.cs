using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    public Vector3 playerCoords;
    public float offset = 50.0f;

    // Start is called before the first frame update
    void Start()
    {
        playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
    }

    // Update is called once per frame
    void Update()
    {
        playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
        this.transform.position = playerCoords + new Vector3(offset, offset - 15, offset);
    }
}
