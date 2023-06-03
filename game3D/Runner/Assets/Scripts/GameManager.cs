using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using TMPro;

public class GameManager : MonoBehaviour
{
    [SerializeField] private GameObject mainMenuCanvas;
    [SerializeField] private GameObject gameCanvas;
    [SerializeField] private GameObject pauseCanvas;
    [SerializeField] private GameObject gameOverCanvas;

    private PlayerMovement playerMovementScript;
    private CreateLevel createLevelScript;

    private static bool gameStarted = false;
    private float gameTime = 0f;

    public bool godMode = false;

    // Start is called before the first frame update
    void Start()
    {
        gameTime = 0f;

        GameObject playerObject = GameObject.Find("Player");
        playerMovementScript = playerObject.GetComponent<PlayerMovement>();
        playerMovementScript.proceduralLevel = true;

        GameObject levelObject = GameObject.Find("Level");
        createLevelScript = levelObject.GetComponent<CreateLevel>();

        if (!gameStarted)
        {
            Time.timeScale = 0f;
            //gameStarted = false;
            mainMenuCanvas.SetActive(true);
            gameCanvas.SetActive(false);
            pauseCanvas.SetActive(false);
            gameOverCanvas.SetActive(false);

            FindObjectOfType<AudioManager>().playSound("MenuSong");
        }
        else
        {
            startGame();
        }
    }

    // Update is called once per frame
    void Update()
    {
        timeManager();

        if (!gameStarted && mainMenuCanvas.activeSelf && Input.GetKeyDown(KeyCode.Space))
        {
                startGame();
        }
        
        if (gameStarted && !pauseCanvas.activeSelf && !gameOverCanvas.activeSelf && (Input.GetKeyDown(KeyCode.Escape) || Input.GetKeyDown(KeyCode.P)))
        {
            pauseGame();
        }
        else if (gameStarted && pauseCanvas.activeSelf && (Input.GetKeyDown(KeyCode.Escape) || Input.GetKeyDown(KeyCode.P)))
        {
            resumeGame();
        }

        if (!playerMovementScript.bAlive)
        {
            gameOver();
        }

        if (Input.GetKeyDown(KeyCode.G))
        {
            playerMovementScript.godMode = !playerMovementScript.godMode;
            godMode = !godMode;
            if (playerMovementScript.godMode)
            {
                Debug.Log("GodMode activated");

                FindObjectOfType<AudioManager>().playSound("pickOption");
            }
            else
            {
                Debug.Log("GodMode deactivated");

                FindObjectOfType<AudioManager>().playSound("pickOption");
            }
        }
    }

    private void timeManager()
    {
        gameTime += Time.deltaTime;
    }

    public void startGame()
    {
        gameStarted = true;

        FindObjectOfType<AudioManager>().stopSound("MenuSong");
        FindObjectOfType<AudioManager>().playSound("MainSong");

        mainMenuCanvas.SetActive(false);
        gameCanvas.SetActive(true);

        FindObjectOfType<AudioManager>().playSound("pickOption");

        Time.timeScale = 1f;
    }

    public void pauseGame()
    {
        Time.timeScale = 0f;

        FindObjectOfType<AudioManager>().playSound("pickOption");

        // Hide pause button
        Transform pauseButtonTransform = gameCanvas.transform.Find("PauseButton");
        SetActiveButton(pauseButtonTransform, false);

        pauseCanvas.SetActive(true);
    }

    public void resumeGame()
    {
        // Show pause button
        Transform pauseButtonTransform = gameCanvas.transform.Find("PauseButton");
        SetActiveButton(pauseButtonTransform, true);

        FindObjectOfType<AudioManager>().playSound("pickOption");

        pauseCanvas.SetActive(false);

        Time.timeScale = 1f;
    }

    public void restartGame()
    {
        SceneManager.LoadScene("Game");

        FindObjectOfType<AudioManager>().playSound("pickOption");
    }

    public void quitGame()
    {
        gameStarted = false;
        SceneManager.LoadScene("Game");

        FindObjectOfType<AudioManager>().playSound("pickOption");
    }

    public void gameOver()
    {
        Time.timeScale = 0f;

        gameOverCanvas.SetActive(true);

        int score = PlayerPrefs.GetInt("ScoreCount", 0);
        int highscore = PlayerPrefs.GetInt("HighscoreCount", 0);

        if (score > highscore)
        {
            PlayerPrefs.SetInt("HighscoreCount", score);
        }

        TextMeshProUGUI scoreText = gameOverCanvas.transform.Find("Score").GetComponent<TextMeshProUGUI>();
        scoreText.text = PlayerPrefs.GetInt("ScoreCount", 0).ToString();

        TextMeshProUGUI highscoreText = gameOverCanvas.transform.Find("Highscore").GetComponent<TextMeshProUGUI>();
        highscoreText.text = PlayerPrefs.GetInt("HighscoreCount", 0).ToString();
    }

    void SetActiveButton(Transform buttonTransform, bool b)
    {
        Button button = buttonTransform.GetComponent<Button>();

        if (button != null)
        {
            button.gameObject.SetActive(b);
        }
    }
}
