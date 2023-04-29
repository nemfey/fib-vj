using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;

    public float velocity = 10f;
    public float turnSpeed = 10f;   // de momento no se usa
    public float jumpSpeed = 5f;
    public float jumpCount = 0;

    float rightTurnAngle = 90f;     // son de verdad necesarios?
    float leftTurnAngle = -90f;      // son de verdad necesarios?
    bool bTurnRight = true;

    float tolerance = 0.1f;
    float smoothness = 1f;
    float centerSection = -12.5f;

    RaycastHit hitInfo;
    bool bGrounded = true;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        moveForward();
        moveToCenter();
        rotate();

        if (Input.GetKeyDown(KeyCode.Space))
        {
            if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
            {
                string collider_tag = hitInfo.collider.tag;
                if ((collider_tag == "Floor" || collider_tag == "Obstacle") && jumpCount < 2)
                {
                    GetComponent<Rigidbody>().AddForce(Vector3.up * jumpSpeed, ForceMode.Impulse);
                    jumpCount++;
                    bGrounded = false;
                }
                else if (collider_tag == "RightTurn" && bTurnRight && bGrounded)
                {
                    transform.Rotate(Vector3.up, rightTurnAngle);

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                    bTurnRight = false;
                    centerSection = hitInfo.collider.bounds.center.z;
                }
                else if (collider_tag == "LeftTurn" && !bTurnRight && bGrounded)
                {
                    transform.Rotate(Vector3.up, leftTurnAngle);

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                    bTurnRight = true;
                    centerSection = hitInfo.collider.bounds.center.x;
                }
                }
        }
    }

    void moveForward()
    {
       transform.position += transform.forward * velocity * Time.deltaTime;
       //transform.Translate(Vector3.forward * velocity * Time.deltaTime);
    }

    void moveToCenter()
    {
        // how far are we from the center
        float axisValue = bTurnRight ? transform.position.x : transform.position.z;
        if (Mathf.Abs(axisValue - centerSection) > tolerance)
        {
            //float newAxisValue = Mathf.Lerp(axisValue, centerSection, Time.deltaTime * smoothness);
            float newAxisValue = Mathf.MoveTowards(axisValue, centerSection, Time.deltaTime * smoothness);

            // Assume we just turned right, hence, center X value
            Vector3 newPosition = new Vector3(newAxisValue, transform.position.y, transform.position.z);
            // if we just turned left, then
            if (!bTurnRight)
                newPosition = new Vector3(transform.position.x, transform.position.y, newAxisValue);
            transform.position = newPosition;
        }
    }

    void rotate()
    {

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
