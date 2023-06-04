using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DragonMovement : MonoBehaviour
{
    private Animator animator;

    private GameManager gameManagerScript;

    GameObject player;
    Vector3 playerCoords;
    float minY;

    float movementSpeed = 7f;


    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
        animator.SetBool("Fly", true);
        animator.Play("Fly", 0, 0f);

        GameObject gameManagerObject = GameObject.Find("GameManager");
        gameManagerScript = gameManagerObject.GetComponent<GameManager>();

        player = GameObject.Find("Player");
        playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
        minY = playerCoords.y;
    }

    
    private void Update()
    {
        if (!gameManagerScript.cutSceneStarted)
        {
            playerCoords = player.GetComponent<Transform>().position;
            minY = (playerCoords.y < minY) ? playerCoords.y : minY;
        }
        else
        {
            GameObject boat = getBoatGameObject();
            playerCoords = boat.GetComponent<Transform>().position;
            minY = playerCoords.y;
        }

        moveToPlayer();
        rotateToPlayer();

        //if (Random.Range(0,100) == 0 && !animator.GetCurrentAnimatorStateInfo(0).IsName("Vox_Dragon_Breath_Fw"))
        //{
        //    animator.Play("Vox_Dragon_Breath_Fw", 0, 0f);
        //}
    }

    private GameObject getBoatGameObject()
    {
        GameObject level = GameObject.Find("Level");
        GameObject wall = level.transform.Find("EndGameWall(Clone)").gameObject;
        GameObject boat = wall.transform.Find("EndGameBoat").gameObject;
        return boat;
    }

    private void moveToPlayer()
    {
        Vector3 targetPosition = playerCoords;
        targetPosition.y = minY;

        Vector3 newPosition = Vector3.MoveTowards(transform.position, targetPosition, movementSpeed * Time.deltaTime);
        transform.position = newPosition;
    }

    private void rotateToPlayer()
    {
        Vector3 targetPosition = playerCoords;
        targetPosition.y = minY;

        Vector3 direction = (targetPosition - transform.position).normalized;
        transform.rotation = Quaternion.LookRotation(direction);
    }
}
