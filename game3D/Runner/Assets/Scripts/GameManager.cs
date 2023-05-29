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

    private static bool gameStarted = false;

    //private static bool  = false;

    // Start is called before the first frame update
    void Start()
    {
        GameObject playerObject = GameObject.Find("Player");
        playerMovementScript = playerObject.GetComponent<PlayerMovement>();

        if (!gameStarted)
        {
            Time.timeScale = 0f;
            //gameStarted = false;
            mainMenuCanvas.SetActive(true);
            gameCanvas.SetActive(false);
            pauseCanvas.SetActive(false);
            gameOverCanvas.SetActive(false);
        }
        else
        {
            startGame();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (!gameStarted && mainMenuCanvas.activeSelf && Input.GetKeyDown(KeyCode.Space))
        {
            startGame();
        }
        
        if (gameStarted && !pauseCanvas.activeSelf && (Input.GetKeyDown(KeyCode.Escape) || Input.GetKeyDown(KeyCode.P)))
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
    }

    public void startGame()
    {
        gameStarted = true;
        FindObjectOfType<AudioManager>().playSound("MainSong");

        mainMenuCanvas.SetActive(false);
        gameCanvas.SetActive(true);

        Time.timeScale = 1f;
    }

    public void pauseGame()
    {
        Time.timeScale = 0f;

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
        
        pauseCanvas.SetActive(false);

        Time.timeScale = 1f;
    }

    public void restartGame()
    {
        SceneManager.LoadScene("Game");
    }

    public void quitGame()
    {
        gameStarted = false;
        SceneManager.LoadScene("Game");
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
