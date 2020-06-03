package mountainhuts;

import java.util.Optional;

/**
 * Represents a mountain hut.
 * 
 * It is linked to a {@link Municipality}
 *
 */
public class MountainHut {
	
	/*
	 * ATTRIBUTE
	 */
	private String name;
	private String category;
	private Optional<Integer> altitude;
	private Integer bedsNumber;
	private Municipality municipality;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param name
	 * @param category
	 * @param altitude
	 * @param bedsNumber
	 * @param municipality
	 */
	public MountainHut(String name, String category, Integer altitude, Integer bedsNumber, Municipality municipality) {
		this.name = name;
		this.category = category;
		this.altitude = Optional.ofNullable(altitude);
		this.bedsNumber = bedsNumber;
		this.municipality = municipality;
	}

	/**
	 * Unique name of the mountain hut
	 * @return name
	 */
	public String getName() {
		return this.name;
	}

	/**
	 * Altitude of the mountain hut.
	 * May be absent, in this case an empty {@link java.util.Optional} is returned.
	 * 
	 * @return optional containing the altitude
	 */
	public Optional<Integer> getAltitude() {
		return this.altitude;
	}

	/**
	 * Category of the hut
	 * 
	 * @return the category
	 */
	public String getCategory() {
		return this.category;
	}

	/**
	 * Number of beds places available in the mountain hut
	 * @return number of beds
	 */
	public Integer getBedsNumber() {
		return this.bedsNumber;
	}

	/**
	 * Municipality where the hut is located
	 *  
	 * @return municipality
	 */
	public Municipality getMunicipality() {
		return this.municipality;
	}

}
