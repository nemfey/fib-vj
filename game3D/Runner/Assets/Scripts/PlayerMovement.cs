using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;

    public float speed;
    public float jumpForce;
    public float jumpCount;
    float rightTurnAngle;
    float leftTurnAngle;

    bool isGrounded; // falta implementar
    bool turnRight;

    float tolerance = 0.1f;
    float smoothness = 1f;
    float centerSection = -12.5f;

    // Start is called before the first frame update
    void Start()
    {
        isGrounded = true;
        turnRight = true;
        speed = 5f;
        jumpForce = 5f;
        jumpCount = 0;
        rightTurnAngle = 90f;
        leftTurnAngle = -90f;
    }

    // Update is called once per frame
    void Update()
    {
        //transform.Translate(Vector3.forward * speed * Time.deltaTime);
        playerMovement();

        if (Input.GetKeyDown(KeyCode.Space))
        {
            RaycastHit hit;
            if (Physics.Raycast(transform.position, Vector3.down, out hit))
            {
                if (hit.collider.tag == "Floor" && jumpCount < 2)
                {
                    GetComponent<Rigidbody>().AddForce(Vector3.up * jumpForce, ForceMode.Impulse);
                    jumpCount++;
                    isGrounded = false;
                }
                else if (hit.collider.tag == "RightTurn" && turnRight)
                {
                    transform.Rotate(Vector3.up, rightTurnAngle);
                    // interpolacion hasta el centro de la carretera

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                    turnRight = false;
                    centerSection += 10f;
                }
                else if (hit.collider.tag == "LeftTurn" && !turnRight)
                {
                    transform.Rotate(Vector3.up, leftTurnAngle);

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                    turnRight = true;
                    centerSection += 15f;

                }
            }
        }
    }

    void playerMovement()
    {
        transform.Translate(Vector3.forward * speed * Time.deltaTime);

        // how far are we from the center
        float axisValue = turnRight ? transform.position.x : transform.position.z;
        if (Mathf.Abs(axisValue - centerSection) > tolerance)
        {
            //float newAxisValue = Mathf.Lerp(axisValue, centerSection, Time.deltaTime * smoothness);
            float newAxisValue = Mathf.MoveTowards(axisValue, centerSection, Time.deltaTime * smoothness);

            // Assume we just turned right, hence, center X value
            Vector3 newPosition = new Vector3(newAxisValue, transform.position.y, transform.position.z);
            // if we just turned left, then
            if (!turnRight)
                newPosition = new Vector3(transform.position.x, transform.position.y, newAxisValue);
            transform.position = newPosition;
        }
    }

    void OnCollisionEnter(Collision c)
    {
        if (c.collider.tag == "Floor")
        {
            jumpCount = 0;
            isGrounded = true;
        }
    }
}
