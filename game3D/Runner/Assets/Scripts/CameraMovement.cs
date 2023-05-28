using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    /*
    public Vector3 playerCoords;
    public float offsetX = 35;
    public float offsetY = 40f;
    public float offsetZ = 40;
    */

    private PlayerMovement playerMovementScript;

    // Camera movement
    Vector3 currentPosition;
    Vector3 targetPosition;

    float movementSpeed = 8f;

    // Start is called before the first frame update
    void Start()
    {
        GameObject playerObject = GameObject.Find("Player");
        playerMovementScript = playerObject.GetComponent<PlayerMovement>();

        currentPosition = this.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        //playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
        //this.transform.position = playerCoords + new Vector3(offsetX, offsetY, offsetZ);

        currentPosition = this.transform.position;

        // Movement
        targetPosition = playerMovementScript.cameraPosition;
        targetPosition.x += 40f;
        targetPosition.z += 40f;
        //targetPosition.x += 50f;
        //targetPosition.z += 50f;

        currentPosition = Vector3.MoveTowards(currentPosition, targetPosition, movementSpeed * Time.deltaTime);
        this.transform.position = currentPosition;

    }
}
