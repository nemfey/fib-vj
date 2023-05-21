using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;
using TMPro;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;

    public Animator animController;

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

    [SerializeField] private GameObject gameCanvas;
    private TextMeshProUGUI scoreText;
    public int score = 0;

    private TextMeshProUGUI coinsText;
    public int coins = 0;

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

        animController = GetComponentInChildren<Animator>();

        scoreText = gameCanvas.transform.Find("Score").GetComponent<TextMeshProUGUI>();
        coinsText = gameCanvas.transform.Find("Coins").GetComponent<TextMeshProUGUI>();
    }

    // Update is called once per frame
    void Update()
    {
        moveForward();
        moveToCenter();
        rotateToTargetAngle();

        if (Input.GetKeyDown(KeyCode.Space))
        {
            if (!bGrounded && jumpCount < 2)
            {
                animController.SetTrigger("DJumpTrigger");

                rb.AddForce(Vector3.up * jumpForce, ForceMode.VelocityChange);
                jumpCount++;
            }
            else if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
            {
                string collider_tag = hitInfo.collider.tag;
                if ((collider_tag == "Floor" || collider_tag == "Obstacle") && jumpCount < 2)
                {
                    rb.AddForce(Vector3.up * jumpForce, ForceMode.VelocityChange);
                    jumpCount++;
                    bGrounded = false;

                    animController.SetBool("InAir", true);
                }
                else if (collider_tag == "RightTurn" && targetAngle == 0f && bGrounded)
                {
                    centerSection = hitInfo.collider.bounds.center.z;
                    targetAngle = 90f;
                    score++;
                    scoreText.text = score.ToString();

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                }
                else if (collider_tag == "LeftTurn" && targetAngle == 90f && bGrounded)
                {
                    centerSection = hitInfo.collider.bounds.center.x;
                    targetAngle = 0f;
                    score++;
                    scoreText.text = score.ToString();

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                }
            }
        }

        if (onSlope() && bGrounded)
        {
            Debug.Log("SLOPEDOWN!");
            //rb.AddForce(Vector3.down * 5f * Time.deltaTime);
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

            animController.SetBool("InAir", false);
        }
        if (collider_tag == "Obstacle")
        {
            Debug.Log("IM DEAD!");
        }
        if (collider_tag == "Coin")
        {
            Destroy(c.gameObject);
            coins++;
            coinsText.text = coins.ToString();
        }
    }
}
