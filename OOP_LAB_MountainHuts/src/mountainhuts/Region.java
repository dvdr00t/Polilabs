package mountainhuts;

import static java.util.stream.Collectors.toList;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Collection;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.TreeMap;
import java.util.stream.Collectors;

/**
 * Class {@code Region} represents the main facade
 * class for the mountains hut system.
 * 
 * It allows defining and retrieving information about
 * municipalities and mountain huts.
 *
 */
public class Region {
	
	/*
	 * ATTRIBUTES
	 */
	private String regionName;
	private List<AltitudeRange> ranges;
	private List<Municipality> munis;
	private List<MountainHut> huts;

	/**
	 * Create a region with the given name.
	 * 
	 * @param name
	 *            the name of the region
	 */
	public Region(String name) {
		this.regionName = name;
		this.ranges = new LinkedList<AltitudeRange>();
		this.munis = new LinkedList<Municipality>();
		this.huts = new LinkedList<MountainHut>();
	}

	/**
	 * Return the name of the region.
	 * 
	 * @return the name of the region
	 */
	public String getName() {
		return this.regionName;
	}

	/**
	 * Create the ranges given their textual representation in the format
	 * "[minValue]-[maxValue]".
	 * 
	 * @param ranges
	 *            an array of textual ranges
	 */
	public void setAltitudeRanges(String... ranges) {
		for (String s: ranges) {
			this.ranges.add(new AltitudeRange(s));
		}

	}

	/**
	 * Return the textual representation in the format "[minValue]-[maxValue]" of
	 * the range including the given altitude or return the default range "0-INF".
	 * 
	 * @param altitude
	 *            the geographical altitude
	 * @return a string representing the range
	 */
	public String getAltitudeRange(Integer altitude) {
		
//		
//		for (AltitudeRange ar: this.ranges)
//			if (altitude >= ar.getMinValue() && altitude < ar.getMaxValue())
//				return ar.getMinValue() + "-" + ar.getMaxValue();
//				
//		return "0-INF";
		
		String result = this.ranges.stream()
				.filter(x-> (x.getMinValue() <= altitude && altitude <= x.getMaxValue()))
				.map(AltitudeRange::toString)
				.findFirst()
				.orElse("0-INF");
		return result;
				
				
				
	}

	/**
	 * Create a new municipality if it is not already available or find it.
	 * Duplicates must be detected by comparing the municipality names.
	 * 
	 * @param name
	 *            the municipality name
	 * @param province
	 *            the municipality province
	 * @param altitude
	 *            the municipality altitude
	 * @return the municipality
	 */
	public Municipality createOrGetMunicipality(String name, String province, Integer altitude) {

		return this.munis.stream()
				.filter(x -> (x.getName().equals(name)))
				.findFirst()
				.orElseGet(() -> {
					
					Municipality newMuni = new Municipality(name, province, altitude);
					this.munis.add(newMuni);
					return newMuni;
				});
	}

	/**
	 * Return all the municipalities available.
	 * 
	 * @return a collection of municipalities
	 */
	public Collection<Municipality> getMunicipalities() {
		Collection<Municipality> toBeReturned = new LinkedList<Municipality>(this.munis);
		return toBeReturned;
	}

	/**
	 * Create a new mountain hut if it is not already available or find it.
	 * Duplicates must be detected by comparing the mountain hut names.
	 *
	 * @param name
	 *            the mountain hut name
	 * @param category
	 *            the mountain hut category
	 * @param bedsNumber
	 *            the number of beds in the mountain hut
	 * @param municipality
	 *            the municipality in which the mountain hut is located
	 * @return the mountain hut
	 */
	public MountainHut createOrGetMountainHut(String name, String category, Integer bedsNumber,
			Municipality municipality) {
		
		return createOrGetMountainHut(name, null, category, bedsNumber, municipality);
	}

	/**
	 * Create a new mountain hut if it is not already available or find it.
	 * Duplicates must be detected by comparing the mountain hut names.
	 * 
	 * @param name
	 *            the mountain hut name
	 * @param altitude
	 *            the mountain hut altitude
	 * @param category
	 *            the mountain hut category
	 * @param bedsNumber
	 *            the number of beds in the mountain hut
	 * @param municipality
	 *            the municipality in which the mountain hut is located
	 * @return a mountain hut
	 */
	public MountainHut createOrGetMountainHut(String name, Integer altitude, String category, Integer bedsNumber,
			Municipality municipality) {

		MountainHut result = this.huts.stream()
				.filter(x -> (x.getName().equals(name)))
				.findFirst()
				.orElseGet(() -> {
					MountainHut newHut = new MountainHut(name, category, altitude, bedsNumber, municipality);
					this.huts.add(newHut);
					return newHut;
				});
		return result;
	}

	/**
	 * Return all the mountain huts available.
	 * 
	 * @return a collection of mountain huts
	 */
	public Collection<MountainHut> getMountainHuts() {
		Collection<MountainHut> toBeReturned = new LinkedList<MountainHut>(this.huts);
		return toBeReturned;
	}

	/**
	 * Factory methods that creates a new region by loading its data from a file.
	 * 
	 * The file must be a CSV file and it must contain the following fields:
	 * <ul>
	 * <li>{@code "Province"},
	 * <li>{@code "Municipality"},
	 * <li>{@code "MunicipalityAltitude"},
	 * <li>{@code "Name"},
	 * <li>{@code "Altitude"},
	 * <li>{@code "Category"},
	 * <li>{@code "BedsNumber"}
	 * </ul>
	 * 
	 * The fields are separated by a semicolon (';'). The field {@code "Altitude"}
	 * may be empty.
	 * 
	 * @param name
	 *            the name of the region
	 * @param file
	 *            the path of the file
	 */
	public static Region fromFile(String name, String file) {
		
		//Creating the object region to be returned and "opening" (creating a list of String) the file 
		Region region = new Region(name);
		List<String> data = readData(file);
		
		//CREATING THE SET OF HEADERS OF THE FILE
		String[] headers = data.remove(0).split(";");	
		Map<String, Integer> map = new HashMap<>();
		for (int i = 0; i < headers.length; i++)
			map.put(headers[i], i);
		
		//READING DATA
		data.forEach(l -> {
			
			//Splitting each line in order to retrieve the data
			String[] values = l.split(";");
			
			//Creating the values for the municipality
			String province = values[map.get("Province")];
			String munisName = values[map.get("Municipality")];
			Integer munisAltitude = Integer.parseInt(values[map.get("MunicipalityAltitude")]);
			
			Municipality newMuni = region.createOrGetMunicipality(munisName, province, munisAltitude);
			
			//Creating the values for the hut
			String hName = values[map.get("Name")];
			String hAltitude = values[map.get("Altitude")];
			String hCategory = values[map.get("Category")];
			Integer hBedsNumber = Integer.parseInt(values[map.get("BedsNumber")]);
			
			if (hAltitude.equals(""))
				region.createOrGetMountainHut(hName, hCategory, hBedsNumber, newMuni);
			else 
				region.createOrGetMountainHut(hName, Integer.parseInt(hAltitude), hCategory, hBedsNumber, newMuni);
				
		});
		
		
		return region;
	}

	/**
	 * Internal class that can be used to read the lines of
	 * a text file into a list of strings.
	 * 
	 * When reading a CSV file remember that the first line
	 * contains the headers, while the real data is contained
	 * in the following lines.
	 * 
	 * @param file the file name
	 * @return a list containing the lines of the file
	 */
	@SuppressWarnings("unused")
	private static List<String> readData(String file) {
		try (BufferedReader in = new BufferedReader(new FileReader(file))) {
			return in.lines().collect(toList());
		} catch (IOException e) {
			System.err.println(e.getMessage());
			return null;
		}
	}

	/**
	 * Count the number of municipalities with at least a mountain hut per each
	 * province.
	 * 
	 * @return a map with the province as key and the number of municipalities as
	 *         value
	 */
	public Map<String, Long> countMunicipalitiesPerProvince() {
		
		return this.munis.stream()
				.collect(Collectors.groupingBy(x -> x.getProvince(), Collectors.counting()));
				
	}

	/**
	 * Count the number of mountain huts per each municipality within each province.
	 * 
	 * @return a map with the province as key and, as value, a map with the
	 *         municipality as key and the number of mountain huts as value
	 */
	public Map<String, Map<String, Long>> countMountainHutsPerMunicipalityPerProvince() {
		return this.huts.stream()
				.collect(Collectors.groupingBy(x -> x.getMunicipality().getProvince(),
						Collectors.groupingBy(z -> z.getMunicipality().getName(), Collectors.counting())));
	}

	/**
	 * Count the number of mountain huts per altitude range. If the altitude of the
	 * mountain hut is not available, use the altitude of its municipality.
	 * 
	 * @return a map with the altitude range as key and the number of mountain huts
	 *         as value
	 */
	public Map<String, Long> countMountainHutsPerAltitudeRange() {
		
		//Creating the hash map
		Map<String, Long> map = new HashMap<>();
		
		//Adding the altitude ranges
		for(AltitudeRange ar: this.ranges)
			map.put(ar.toString(), (long) 0);
		map.put("0-INF", (long) 0);
		
		
		for (MountainHut hut: this.huts) {
			
			//Retrieving the altitude of the hut
			Integer altitude = 0;
			
			if (hut.getAltitude().isPresent())
				altitude = hut.getAltitude().get();
			else
				altitude = hut.getMunicipality().getAltitude();
			
			
			map.replace(this.getAltitudeRange(altitude), map.get(this.getAltitudeRange(altitude))+1);
		}
		
		return map;
	}

	/**
	 * Compute the total number of beds available in the mountain huts per each
	 * province.
	 * 
	 * @return a map with the province as key and the total number of beds as value
	 */
	public Map<String, Integer> totalBedsNumberPerProvince() {
		return this.huts.stream()
				.collect(Collectors.groupingBy(x -> x.getMunicipality().getProvince(), 
						Collectors.summingInt(MountainHut::getBedsNumber)));
	}

	/**
	 * Compute the maximum number of beds available in a single mountain hut per
	 * altitude range. If the altitude of the mountain hut is not available, use the
	 * altitude of its municipality.
	 * 
	 * @return a map with the altitude range as key and the maximum number of beds
	 *         as value
	 */
	public Map<String, Optional<Integer>> maximumBedsNumberPerAltitudeRange() {
		
		Map<String, Optional<Integer>> res = this.huts.stream()
				.collect(Collectors.groupingBy(x -> getAltitudeRange(x),
						Collectors.mapping(MountainHut::getBedsNumber,
								Collectors.maxBy(Comparator.naturalOrder()))));
		
		this.ranges.stream()
		.map(AltitudeRange::toString)
		.forEach(x -> res.putIfAbsent(x, Optional.of(0)));
		
		return res;
	}

	/**
	 * Compute the municipality names per number of mountain huts in a municipality.
	 * The lists of municipality names must be in alphabetical order.
	 * 
	 * @return a map with the number of mountain huts in a municipality as key and a
	 *         list of municipality names as value
	 */
	public Map<Long, List<String>> municipalityNamesPerCountOfMountainHuts() {
		Map<String, Integer> m = new HashMap<>();
		m.entrySet();
		return this.huts.stream()
		.collect(Collectors.groupingBy(x -> x.getMunicipality().getName(), 
				() -> new TreeMap<String, Long>(Comparator.naturalOrder()),
				Collectors.counting()))
		.entrySet().stream() 
		.collect(Collectors.groupingBy(x -> x.getValue(),
				Collectors.mapping(x -> x.getKey(), Collectors.toList())));
	}

	
	private String getAltitudeRange(MountainHut m) {
		if (m.getAltitude().isPresent()) {
			return getAltitudeRange(m.getAltitude().get());
		} else {
			return getAltitudeRange(m.getMunicipality().getAltitude());
		}
	}
}
