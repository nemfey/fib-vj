using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;

    public float velocity = 10f;

    [SerializeField] Rigidbody rb;
    public float jumpForce = 15f;
    public float jumpCount = 0;

    float targetAngle = 0f;
    float turnSpeed = 720f;

    float smoothness = 2f;
    float centerSection = -12.5f;

    RaycastHit hitInfo;
    bool bGrounded = true;

    public int score = 0;

    private bool onSlope()
    {
        if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
        {
            return hitInfo.normal != Vector3.up;
        }
        return false;
    }

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        moveForward();
        moveToCenter();
        rotateToTargetAngle();

        if (Input.GetKeyDown(KeyCode.Space))
        {
            if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
            {
                string collider_tag = hitInfo.collider.tag;
                if ((collider_tag == "Floor" || collider_tag == "Obstacle") && jumpCount < 2)
                {
                    rb.AddForce(Vector3.up * jumpForce, ForceMode.VelocityChange);
                    jumpCount++;
                    bGrounded = false;
                }
                else if (collider_tag == "RightTurn" && targetAngle == 0f && bGrounded)
                {
                    centerSection = hitInfo.collider.bounds.center.z;
                    targetAngle = 90f;
                    score++;

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                }
                else if (collider_tag == "LeftTurn" && targetAngle == 90f && bGrounded)
                {
                    centerSection = hitInfo.collider.bounds.center.x;
                    targetAngle = 0f;
                    score++;
                    
                    level.GetComponent<CreateLevel>().newSectionProcedure();
                }
            }
        }

        if (onSlope() && bGrounded)
        {
            Debug.Log("SLOPEDOWN!");
            rb.AddForce(Vector3.down * 5f * Time.deltaTime);
            // player should be touchingh the slope all the time
            // instead of floating
        }
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

    void OnCollisionEnter(Collision c)
    {
        string collider_tag = c.collider.tag;
        if (collider_tag == "Floor" || collider_tag == "RightTurn" || collider_tag == "LeftTurn")
        {
            jumpCount = 0;
            bGrounded = true;
        }
        if (collider_tag == "Obstacle")
        {
            Debug.Log("IM DEAD!");
        }
    }
}
