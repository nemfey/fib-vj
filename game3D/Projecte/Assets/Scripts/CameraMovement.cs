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

    private GameManager gameManagerScript;
    private PlayerMovement playerMovementScript;
    private CreateLevel createLevelScript;

    // Camera movement
    Vector3 currentPosition;
    Vector3 targetPosition;

    float movementSpeed = 8f;

    // Start is called before the first frame update
    void Start()
    {
        GameObject gameManagerObject = GameObject.Find("GameManager");
        gameManagerScript = gameManagerObject.GetComponent<GameManager>();

        GameObject playerObject = GameObject.Find("Player");
        playerMovementScript = playerObject.GetComponent<PlayerMovement>();

        GameObject levelObject = GameObject.Find("Level");
        createLevelScript = levelObject.GetComponent<CreateLevel>();

        currentPosition = this.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        currentPosition = this.transform.position;

        if (!gameManagerScript.cutSceneStarted)
        {
            movementSpeed = playerMovementScript.cameraSpeed;
            targetPosition = playerMovementScript.cameraPosition;

            targetPosition.x += 40f;
            targetPosition.y += createLevelScript.currentChunkY;
            targetPosition.z += 40f;
            //targetPosition.x += 50f;
            //targetPosition.z += 50f;

        }
        else if (gameManagerScript.cutSceneStarted)
        {
            movementSpeed = 50f;
            GameObject boat = getBoatGameObject();
            targetPosition = boat.GetComponent<Transform>().position;
            Debug.Log(targetPosition);

            float targetY = targetPosition.y + 50f;
            if (targetY < currentPosition.y)
            {
                targetY = currentPosition.y;
            }
            targetPosition.x += 40f;
            targetPosition.y = targetY;
            targetPosition.z += 40f;
        }

        currentPosition = Vector3.MoveTowards(currentPosition, targetPosition, movementSpeed * Time.deltaTime);
        this.transform.position = currentPosition;

    }

    private GameObject getBoatGameObject()
    {
        GameObject level = GameObject.Find("Level");
        GameObject wall = level.transform.Find("EndGameWall(Clone)").gameObject;
        GameObject boat = wall.transform.Find("EndGameBoat").gameObject;
        return boat;
    }
}
