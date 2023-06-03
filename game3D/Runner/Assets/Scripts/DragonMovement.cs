using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DragonMovement : MonoBehaviour
{
    private Animator animator;

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

        player = GameObject.Find("Player");
        playerCoords = GameObject.Find("Player").GetComponent<Transform>().position;
        minY = playerCoords.y;
    }

    
    private void Update()
    {
        playerCoords = player.GetComponent<Transform>().position;
        minY = (playerCoords.y < minY) ? playerCoords.y : minY;
        moveToPlayer();
        rotateToPlayer();

        //if (Random.Range(0,100) == 0 && !animator.GetCurrentAnimatorStateInfo(0).IsName("Vox_Dragon_Breath_Fw"))
        //{
        //    animator.Play("Vox_Dragon_Breath_Fw", 0, 0f);
        //}
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
    /*
    // Update is called once per frame
    private void Update()
    {
        moveForward();
        moveToCenter();
        rotateToTargetAngle();
        turn();
    }

    void moveForward()
    {
        transform.position += transform.forward * velocity * Time.deltaTime;
    }

    void moveToCenter()
    {
        // how far are we from the center
        float axisValue = targetAngle == 0f ? transform.position.x : transform.position.z;
        float newAxisValue = Mathf.MoveTowards(axisValue, centerSection, Time.deltaTime * smoothness);

        // Assume we just turned right, hence, center X value
        Vector3 newPosition = new Vector3(newAxisValue, transform.position.y, transform.position.z);
        // if we just turned left, then
        if (targetAngle == 90f)
            newPosition = new Vector3(transform.position.x, transform.position.y, newAxisValue);
        transform.position = newPosition;
    }

    void rotateToTargetAngle()
    {
        Quaternion targetRotation = Quaternion.Euler(0, targetAngle, 0);
        transform.rotation = Quaternion.RotateTowards(transform.rotation, targetRotation, turnSpeed * Time.deltaTime);
    }

    void turn()
    {
        // Check floor below
        if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
        {
            string collider_tag = hitInfo.collider.tag;
            if (((collider_tag == "RightTurn" && targetAngle == 0f) || (collider_tag == "LeftTurn" && targetAngle == 90f)))
            {
                if (collider_tag == "RightTurn")
                {
                    centerSection = hitInfo.collider.bounds.center.z;
                    targetAngle = 90f;
                }
                else
                {
                    centerSection = hitInfo.collider.bounds.center.x;
                    targetAngle = 0f;
                }
            }
        }
    }
    */
    
}
