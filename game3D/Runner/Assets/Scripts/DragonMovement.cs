using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DragonMovement : MonoBehaviour
{
    private Animator animator;

    public float velocity = 9.5f;

    float targetAngle = 0f;
    float turnSpeed = 720f;

    float smoothness = 10f;
    float centerSection = -12.5f;

    RaycastHit hitInfo;

    // Start is called before the first frame update
    void Start()
    {
        Animation animation = GetComponent<Animation>();

        animator.SetBool("Fly", true);
        animator.Play("Fly", 0, 0f);
    }

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
}
