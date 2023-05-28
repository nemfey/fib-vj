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

    // Camera rotation
    GameObject playerObject;

    // Camera movement
    Vector3 currentPosition;
    Vector3 targetPosition;

    float movementSpeed = 10f;
    float rotationSpeed = 5f;

    // Start is called before the first frame update
    void Start()
    {
        playerObject = GameObject.Find("Player");
        playerMovementScript = playerObject.GetComponent<PlayerMovement>();

        currentPosition = this.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        //playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
        //this.transform.position = playerCoords + new Vector3(offsetX, offsetY, offsetZ);

        currentPosition = this.transform.position;

        // Rotation
        Vector3 targetDirection = playerObject.transform.position - currentPosition;
        float singleStep = rotationSpeed * Time.deltaTime;
        Vector3 newDirection = Vector3.RotateTowards(transform.forward, targetDirection, singleStep, 0.0f);
        this.transform.rotation = Quaternion.LookRotation(newDirection);

        // Movement
        targetPosition = playerMovementScript.cameraPosition;

        currentPosition = Vector3.MoveTowards(currentPosition, targetPosition, movementSpeed * Time.deltaTime);
        this.transform.position = currentPosition;

    }
}
